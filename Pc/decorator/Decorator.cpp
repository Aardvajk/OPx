#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"
#include "visitors/TakesAddrVisitor.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"
#include "types/TypeCompare.h"

#include "decorator/FuncDecorator.h"
#include "decorator/ExprDecorator.h"
#include "decorator/ClassDecorator.h"
#include "decorator/CommonDecorator.h"
#include "decorator/ClassMethodDecorator.h"

#include <pcx/scoped_counter.h>

namespace
{

Sym *searchNamespace(Context &c, NamespaceNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Namespace)
        {
            throw Error(node.location(), "namespace expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

Sym *searchClass(Context &c, ClassNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Class)
        {
            throw Error(node.location(), "class expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

void decorateFunctionBody(Context &c, FuncNode &node, Sym *sym)
{
    if(sym->getProperty("defined").value<bool>())
    {
        throw Error(node.location(), "already defined - ", sym->fullname());
    }

    sym->setProperty("defined", true);

    auto g = c.tree.open(sym);

    for(auto &a: node.args)
    {
        Decorator d(c);
        a->accept(d);
    }

    FuncDecorator fd(c);

    for(auto &i: node.inits)
    {
        i->accept(fd);
    }

    node.body->accept(fd);
}

}

Decorator::Decorator(Context &c) : c(c)
{
}

void Decorator::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Decorator::visit(NamespaceNode &node)
{
    auto sym = searchNamespace(c, node);
    if(!sym)
    {
        auto name = c.assertSimpleNameUnique(node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.name->location(), name));
    }

    node.setProperty("sym", sym);

    auto g = c.tree.open(sym);
    node.body->accept(*this);
}

void Decorator::visit(ClassNode &node)
{
    auto sym = searchClass(c, node);
    if(!sym)
    {
        auto name = c.assertSimpleNameUnique(node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.name->location(), name));
    }

    sym->setProperty("type", c.types.insert(Type::makePrimary(0, sym)));

    node.setProperty("sym", sym);

    if(node.body)
    {
        auto dg = pcx::scoped_counter(c.classDepth);

        if(sym->getProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto g = c.tree.open(sym);

        sym->setProperty("size", std::size_t(0));

        ClassDecorator cd(c);
        node.body->accept(cd);

        ClassMethodDecorator md(c, sym);
        node.body->accept(md);
    }

    if(!c.classDepth)
    {
        for(auto d: c.deferredMethods)
        {
            decorateFunctionBody(c, *d, d->property<Sym*>("sym"));
        }

        c.deferredMethods.clear();
    }
}

void Decorator::visit(VarNode &node)
{
    Type *type = nullptr;

    if(node.type)
    {
        type = TypeBuilder::type(c, node.type.get());
    }

    for(auto &p: node.params)
    {
        ExprDecorator ed(c, type);
        p->accept(ed);
    }

    if(node.value)
    {
        ExprDecorator ed(c, type);
        node.value->accept(ed);

        auto valueType = TypeVisitor::type(c, node.value.get());

        if(type && !type->constant && valueType->constant)
        {
            throw Error(node.location(), "cannot assign const to mutable - ",  NameVisitors::prettyName(node.name.get()));
        }

        if(!type)
        {
            type = valueType;
            if(type->sub && c.tree.current()->container()->type() == Sym::Type::Func)
            {
                type = c.types.insert(Type::removeSub(*type));
            }

            if(type->ref && TakesAddrVisitor::examine(*node.value))
            {
                auto t = *type;
                t.ref = false;

                type = c.types.insert(t);
            }
        }
    }

    if(!type)
    {
        throw Error(node.location(), "type missing - ", NameVisitors::prettyName(node.name.get()));
    }

    if(type->ref && type->sub)
    {
        throw Error(node.location(), "arrays of references not supported - ", NameVisitors::prettyName(node.name.get()));
    }

    auto name = c.assertSimpleNameUnique(node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.name->location(), name));
    sym->setProperty("type", type);

    node.setProperty("sym", sym);
}

void Decorator::visit(FuncNode &node)
{
    auto sym = CommonDecorator::decorateFuncSignature(c, node);
    node.setProperty("sym", sym);

    if(node.body)
    {
        c.scopes = 0;
        decorateFunctionBody(c, node, sym);
    }
}

void Decorator::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}

#include "Decorator.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"

#include "decorator/TypeDecorator.h"
#include "decorator/VarDecorator.h"
#include "decorator/FuncDecorator.h"

#include <pcx/scoped_counter.h>

namespace
{

Sym *search(Context &c, Sym::Type type, Node *node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != type)
        {
            throw Error(node->location(), Sym::toString(type), " expected - ", s->fullname());
        }

        return s;
    }

    return nullptr;
}

Sym *searchFunc(Context &c, Node *node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node->location(), "function expected - ", s->fullname());
        }

        auto t = s->property<Type*>("type");
        if(TypeCompare(c).exactArgs(type, t) && type->constMethod == t->constMethod)
        {
            return s;
        }
    }

    return nullptr;
}

void decorateFunctionBody(Context &c, FuncNode &node, Sym *sym)
{
    if(sym->findProperty("defined").value<bool>())
    {
        throw Error(node.location(), "already defined - ", sym->fullname());
    }

    sym->setProperty("defined", true);

    c.funcInfos.push_back(new FuncInfo());
    sym->setProperty("info", c.funcInfos.back_ptr());

    auto sg = c.tree.open(sym);

    for(auto &a: node.args)
    {
        Visitor::visit<VarDecorator>(a.get(), c);
    }

    Visitor::visit<FuncDecorator>(node.body.get(), c);

    auto t = sym->property<Type*>("type")->returnType;
    if(!TypeCompare(c).compatible(t, c.types.nullType()) && !sym->findProperty("returned").value<bool>())
    {
        throw Error(node.location(), "function must return ", t->text());
    }
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
    auto sym = search(c, Sym::Type::Namespace, node.name.get());
    if(!sym)
    {
        auto n = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.name->location(), n));
    }

    node.setProperty("sym", sym);

    auto sg = c.tree.open(sym);
    node.body->accept(*this);
}

void Decorator::visit(FuncNode &node)
{
    for(auto &a: node.args)
    {
        Visitor::visit<TypeDecorator>(a.get(), c);
    }

    if(node.type)
    {
        Visitor::visit<TypeDecorator>(node.type.get(), c);
    }

    auto t = Type::makeFunction(node.type ? TypeVisitor::assertType(c, node.type.get()) : c.types.nullType());

    t.method = c.tree.current()->type() == Sym::Type::Class;
    t.constMethod = node.constMethod;

    for(auto &a: node.args)
    {
        t.args.push_back(TypeVisitor::assertType(c, a.get()));
    }

    auto type = c.types.insert(t);

    auto sym = searchFunc(c, node.name.get(), type);
    if(sym)
    {
        if(!TypeCompare(c).exact(sym->property<Type*>("type")->returnType, type->returnType))
        {
            throw Error(node.location(), "mismatched return type - ", node.name->description());
        }
    }
    else
    {
        auto n = NameVisitors::assertSimpleName(c, node.name.get());

        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.name->location(), n));
        sym->setProperty("type", type);
    }

    sym->setProperty("method", t.method);

    if(t.constMethod && sym->parent()->type() != Sym::Type::Class)
    {
        throw Error(node.location(), "cannot be const - ", node.name->description());
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(t.method && c.classDepth)
        {
            c.deferredMethods.push_back(&node);
        }
        else
        {
            decorateFunctionBody(c, node, sym);
        }
    }
}

void Decorator::visit(ClassNode &node)
{
    auto sym = search(c, Sym::Type::Class, node.name.get());
    if(!sym)
    {
        auto n = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.name->location(), n));
    }

    auto type = c.types.insert(Type::makePrimary(sym));

    sym->setProperty("type", type);
    sym->setProperty("primitive", Primitive::Type::Invalid);

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->findProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto sg = c.tree.open(sym);
        auto dg = pcx::scoped_counter(c.classDepth);

        node.body->accept(*this);
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
    Visitor::visit<VarDecorator>(&node, c);
}

void Decorator::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}

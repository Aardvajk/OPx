#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/VarNode.h"
#include "nodes/FuncNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"
#include "types/TypeCompare.h"

#include "decorator/FuncDecorator.h"
#include "decorator/ExprDecorator.h"
#include "decorator/ClassDecorator.h"

namespace
{

Sym *searchNamespace(Context &c, NamespaceNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

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
    SymFinder::find(SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

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

Sym *searchFunction(Context &c, FuncNode &node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(SymFinder::Type::Local, c.tree.current(), node.name.get(), sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node.location(), "function expected - ", s->fullname());
        }

        if(TypeCompare::args(type, s->property<const Type*>("type")))
        {
            return s;
        }
    }

    return nullptr;
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

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->getProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto g = c.tree.open(sym);

        ClassDecorator cd(c);
        node.body->accept(cd);
    }
}

void Decorator::visit(VarNode &node)
{
    Type *type = nullptr;

    if(node.type)
    {
        type = TypeBuilder::type(c, node.type.get());
    }

    if(node.value)
    {
        ExprDecorator ed(c, type);
        node.value->accept(ed);

        if(!type)
        {
            type = TypeVisitor::type(c, node.value.get());
        }
    }

    if(!type)
    {
        throw Error(node.location(), "type missing - ", NameVisitors::prettyName(node.name.get()));
    }

    auto name = c.assertSimpleNameUnique(node.name.get());

    auto sym = c.tree.current()->add(new Sym(Sym::Type::Var, node.name->location(), name));
    sym->setProperty("type", type);

    node.setProperty("sym", sym);
}

void Decorator::visit(FuncNode &node)
{
    auto t = Type::makeFunction(0, node.type ? TypeBuilder::type(c, node.type.get()) : c.types.nullType());
    for(auto &a: node.args)
    {
        t.args.push_back(TypeVisitor::type(c, a.get()));
    }

    auto type = c.types.insert(t);

    Sym *sym = searchFunction(c, node, type);
    if(sym)
    {
        if(!TypeCompare::exact(type->returnType, sym->property<const Type*>("type")->returnType))
        {
            throw Error(node.location(), "mismatched returns");
        }
    }
    else
    {
        auto name = c.assertSimpleName(node.name.get());

        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));
        sym->setProperty("type", type);
    }

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->getProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto g = c.tree.open(sym);

        for(auto &a: node.args)
        {
            a->accept(*this);
        }

        FuncDecorator fd(c);
        node.body->accept(fd);
    }
}

#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

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

        if(TypeCompare(c).exactArgs(type, s->property<Type*>("type")))
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
    TypeDecorator td(c);

    for(auto &a: node.args)
    {
        a->accept(td);
    }

    if(node.type)
    {
        node.type->accept(td);
    }

    auto t = Type::makeFunction(node.type ? Visitor::query<TypeVisitor, Type*>(node.type.get()) : c.types.nullType());
    for(auto &a: node.args)
    {
        t.args.push_back(Visitor::query<TypeVisitor, Type*>(a.get()));
    }

    auto type = c.types.insert(c, t);

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

    node.setProperty("type", type);
    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->findProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto sg = c.tree.open(sym);
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

    node.setProperty("sym", sym);

    if(node.body)
    {
        if(sym->findProperty("defined").value<bool>())
        {
            throw Error(node.location(), "already defined - ", sym->fullname());
        }

        sym->setProperty("defined", true);

        auto sg = c.tree.open(sym);
        node.body->accept(*this);
    }
}

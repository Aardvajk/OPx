#include "Decorator.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/FuncNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "decorate/ArgDecorator.h"

#include <pcx/scoped_push.h>

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

Sym *searchFunction(Context &c, Node *node, const Type *type)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Local, c.tree.current(), node, sv);

    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(node->location(), "function expected - ", s->fullname());
        }

        if(TypeCompare::exact(type->args, s->property<Type*>("type")->args))
        {
            return s;
        }
    }

    return nullptr;
}

GenericParamList makeGenericParams(NodeList &nodes)
{
    GenericParamList gp;
    for(auto &n: nodes)
    {
        gp.push_back(GenericParam(Visitor::query<NameVisitors::GenericTagName, std::string>(n.get())));
    }

    return gp;
}

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
        auto name = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Namespace, node.location(), name));
    }

    node.setProperty("sym", sym);

    auto sg = c.tree.open(sym);
    node.body->accept(*this);
}

void Decorator::visit(ClassNode &node)
{
    auto sym = search(c, Sym::Type::Class, node.name.get());
    if(!sym)
    {
        auto name = NameVisitors::assertSimpleUniqueName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Class, node.location(), name));

        sym->setProperty("type", c.types.insert(Type::makePrimary(sym)));
    }

    node.setProperty("sym", sym);

    if(!node.genericTags.empty())
    {
        sym->setProperty("genericParams", makeGenericParams(node.genericTags));
    }
}

void Decorator::visit(FuncNode &node)
{
    GenericParamList generics;
    if(!node.genericTags.empty())
    {
        generics = makeGenericParams(node.genericTags);
    }

    auto gp = pcx::scoped_push(c.generics, generics);

    for(auto &a: node.args)
    {
        Visitor::visit<ArgDecorator>(a.get(), c);
    }

    if(node.type)
    {
        node.type->setProperty("type", Visitor::query<TypeBuilder, Type*>(node.type.get(), c));
    }

    auto t = Type::makeFunction(node.type ? TypeVisitor::assertType(c, node.type.get()) : c.types.nullType());

    for(auto &a: node.args)
    {
        t.args.push_back(TypeVisitor::assertType(c, a.get()));
    }

    auto type = c.types.insert(t);

    auto sym = searchFunction(c, node.name.get(), type);
    if(sym)
    {
        if(!TypeCompare::exact(type->returnType, sym->property<Type*>("type")->returnType))
        {
            throw Error(node.location(), "mismatched return type - ", node.description());
        }

        if(auto g = sym->findProperty("genericParams"))
        {
            if(g.to<GenericParamList>().size() != generics.size())
            {
                throw Error(node.location(), "mismatched generics - ", node.description());
            }
        }
    }
    else
    {
        auto name = NameVisitors::assertSimpleName(c, node.name.get());
        sym = c.tree.current()->add(new Sym(Sym::Type::Func, node.location(), name));

        sym->setProperty("type", type);
    }

    node.setProperty("sym", sym);

    if(!generics.empty())
    {
        sym->setProperty("genericParams", generics);
    }
}

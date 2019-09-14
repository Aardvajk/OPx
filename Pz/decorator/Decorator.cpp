#include "Decorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"
#include "types/TypeBuilder.h"

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

void Decorator::visit(VarNode &node)
{
}

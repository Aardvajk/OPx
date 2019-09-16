#include "Finaliser.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"

#include "types/Type.h"

Finaliser::Finaliser(Context &c) : c(c)
{
}

void Finaliser::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Finaliser::visit(NamespaceNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Finaliser::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sym = node.property<Sym*>("sym");

        if(true)
        {
            auto sg = c.tree.open(sym);
            node.body->accept(*this);
        }

        std::size_t sz = 0;
        for(auto s: sym->children())
        {
            if(s->type() == Sym::Type::Var)
            {
                s->setProperty("offset", sz);
                sz += Type::assertSize(s->location(), s->property<Type*>("type"));
            }
        }

        sym->setProperty("size", sz);
    }
}

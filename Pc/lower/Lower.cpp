#include "Lower.h"


#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/ClassNode.h"
#include "nodes/FuncNode.h"

#include "types/Type.h"

#include "lower/FuncLower.h"

Lower::Lower(Context &c) : c(c)
{
}

void Lower::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void Lower::visit(NamespaceNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Lower::visit(ClassNode &node)
{
    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Lower::visit(FuncNode &node)
{
    if(node.body)
    {
        auto g = c.tree.open(node.property<Sym*>("sym"));

        FuncLower fl(c);

        for(auto &a: node.args)
        {
            a->accept(fl);
        }

        if(node.initialisers)
        {
            node.initialisers->accept(fl);
        }

        node.body->accept(fl);
    }
}

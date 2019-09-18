#include "Lower.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ClassNode.h"

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
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void Lower::visit(FuncNode &node)
{
    if(node.body)
    {
        Visitor::visit<FuncLower>(&node, c);
    }
}

void Lower::visit(ClassNode &node)
{
    if(node.body)
    {
        auto sg = c.tree.open(node.property<Sym*>("sym"));
        node.body->accept(*this);
    }
}

void Lower::visit(PragmaNode &node)
{
    Pragmas::execute(c, node);
}

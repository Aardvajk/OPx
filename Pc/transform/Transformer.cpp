#include "Transformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/NamespaceNode.h"
#include "nodes/FuncNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"

#include "transform/ExprTransformer.h"

Transformer::Transformer(Context &c) : c(c)
{
}

void Transformer::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void Transformer::visit(NamespaceNode &node)
{
    node.body->accept(*this);
}

void Transformer::visit(FuncNode &node)
{
    if(node.body)
    {
        node.body->accept(*this);
    }
}

void Transformer::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void Transformer::visit(ExprNode &node)
{
    if(auto r = ExprTransformer::transform(c, node.expr))
    {
        node.expr = r;
    }
}

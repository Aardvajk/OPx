#include "FuncTransformer.h"

#include "nodes/BlockNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"

#include "transform/ExprTransformer.h"

FuncTransformer::FuncTransformer(Context &c) : c(c)
{
}

void FuncTransformer::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n.accept(*this);
    }
}

void FuncTransformer::visit(VarNode &node)
{
}

void FuncTransformer::visit(ScopeNode &node)
{
    node.body->accept(*this);
}

void FuncTransformer::visit(ExprNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}

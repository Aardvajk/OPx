#include "FuncTransform.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"

#include "transform/ExprTransform.h"

FuncTransform::FuncTransform(Context &c) : c(c)
{
}

void FuncTransform::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncTransform::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncTransform::visit(VarNode &node)
{
}

void FuncTransform::visit(ExprNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

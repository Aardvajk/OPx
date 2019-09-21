#include "FuncTransform.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"

#include "transform/ExprTransform.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

FuncTransform::FuncTransform(Context &c) : c(c), index(0)
{
}

void FuncTransform::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        index = i;
        node.nodes[i]->accept(*this);
    }
}

void FuncTransform::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncTransform::visit(VarNode &node)
{
    if(node.value)
    {
        node.value = ExprTransform::transform(c, node.value);
    }
}

void FuncTransform::visit(ExprNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void FuncTransform::visit(ReturnNode &node)
{
    if(node.expr)
    {
        node.expr = ExprTransform::transform(c, node.expr);
    }
}

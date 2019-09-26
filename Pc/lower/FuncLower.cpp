#include "FuncLower.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/InitNode.h"

#include "lower/ExprLower.h"

FuncLower::FuncLower(Context &c) : c(c)
{
}

void FuncLower::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncLower::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncLower::visit(VarNode &node)
{
    if(node.value)
    {
        node.value = ExprLower::lower(c, node.value, node.property<Sym*>("sym")->property<Type*>("type"));
    }
}

void FuncLower::visit(ExprNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

void FuncLower::visit(ReturnNode &node)
{
    if(node.expr)
    {
        node.expr = ExprLower::lower(c, node.expr);
    }
}

void FuncLower::visit(InitNode &node)
{
    node.target = ExprLower::lower(c, node.target);

    for(auto &p: node.params)
    {
        p = ExprLower::lower(c, p);
    }
}

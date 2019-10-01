#include "FuncLower.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/InitNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"

#include "types/Type.h"

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
        auto rt = c.tree.current()->container()->property<Type*>("type")->returnType;

        node.expr = ExprLower::lower(c, node.expr, rt);
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

void FuncLower::visit(WhileNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
    node.body->accept(*this);
}

void FuncLower::visit(IfNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}

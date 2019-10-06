#include "FuncConvert.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/VarNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/InitNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"
#include "nodes/ForNode.h"

#include "types/Type.h"

#include "convert/ExprConvert.h"
#include "convert/CommonConvert.h"

#include "visitors/TypeVisitor.h"

FuncConvert::FuncConvert(Context &c) : c(c)
{
}

void FuncConvert::visit(BlockNode &node)
{
    for(auto &n: node.nodes)
    {
        n->accept(*this);
    }
}

void FuncConvert::visit(ScopeNode &node)
{
    auto sg = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncConvert::visit(VarNode &node)
{
    if(node.value)
    {
        node.value = ExprConvert::convert(c, node.value);
    }
}

void FuncConvert::visit(ExprNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void FuncConvert::visit(ReturnNode &node)
{
    if(node.expr)
    {
        node.expr = ExprConvert::convert(c, node.expr);
    }
}

void FuncConvert::visit(InitNode &node)
{
    node.target = ExprConvert::convert(c, node.target);

    for(auto &p: node.params)
    {
        p = ExprConvert::convert(c, p);
    }
}

void FuncConvert::visit(WhileNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
    node.body->accept(*this);
}

void FuncConvert::visit(IfNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
    node.body->accept(*this);

    if(node.elseBody)
    {
        node.elseBody->accept(*this);
    }
}

void FuncConvert::visit(ForNode &node)
{
    if(node.init)
    {
        node.init = ExprConvert::convert(c, node.init);
    }

    if(node.cond)
    {
        node.cond = ExprConvert::convert(c, node.cond);
    }

    if(node.post)
    {
        node.post = ExprConvert::convert(c, node.post);
    }

    node.body->accept(*this);
}

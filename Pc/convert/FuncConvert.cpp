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
        node.value = ExprConvert::convert(c, node.value, TypeVisitor::assertType(c, &node));
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
        auto rt = c.tree.current()->container()->property<Type*>("type")->returnType;
        node.expr = ExprConvert::convert(c, node.expr, rt);
    }
}

void FuncConvert::visit(InitNode &node)
{
    node.target = ExprConvert::convert(c, node.target);

    auto t = TypeVisitor::assertType(c, node.target.get());

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprConvert::convert(c, node.params[i], t->args[i]);
    }
}

void FuncConvert::visit(WhileNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr, c.types.boolType());
    node.body->accept(*this);
}

void FuncConvert::visit(IfNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr, c.types.boolType());
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
        node.cond = ExprConvert::convert(c, node.cond, c.types.nullType());
    }

    if(node.post)
    {
        node.post = ExprConvert::convert(c, node.post);
    }

    node.body->accept(*this);
}

#include "FuncLower.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/ScopeNode.h"
#include "nodes/ExprNode.h"
#include "nodes/ReturnNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/WhileNode.h"
#include "nodes/IfNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

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

void FuncLower::visit(VarNode &node)
{
    auto sym = node.property<Sym*>("sym");
    auto type = sym->property<Type*>("type");

    if(type->ref)
    {
        sym->setProperty("type", c.types.insert(type->refToPtr()));
    }
}

void FuncLower::visit(ScopeNode &node)
{
    auto g = c.tree.open(node.property<Sym*>("sym"));
    node.body->accept(*this);
}

void FuncLower::visit(ExprNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

void FuncLower::visit(ReturnNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
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

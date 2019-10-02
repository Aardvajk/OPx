#include "ExprLower.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/CommaNode.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

ExprLower::ExprLower(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprLower::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprLower::lower(c, node.parent);
    }

    if(auto type = TypeVisitor::queryType(c, &node))
    {
        if(!type->ref)
        {
            if(expectedType && expectedType->ref)
            {
                rn = new AddrOfNode(node.location(), cn);
                rn->setProperty("type", c.types.insert(type->addPointer()));
            }
        }
        else
        {
            if(!expectedType || !expectedType->ref)
            {
                rn = new DerefNode(node.location(), cn);
                rn->setProperty("type", c.types.insert(type->removePointer()));
            }
        }
    }
}

void ExprLower::visit(CallNode &node)
{
    auto type = TypeVisitor::assertType(c, node.target.get());

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprLower::lower(c, node.params[i], type->args[i]);
    }

    if(type->returnType->ref)
    {
        if(!expectedType || !expectedType->ref)
        {
            rn = new DerefNode(node.location(), cn);
            rn->setProperty("type", c.types.insert(type->returnType->removePointer()));
        }
    }
}

void ExprLower::visit(ConstructNode &node)
{
    if(node.type->primitive())
    {
        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprLower::lower(c, node.params[i]);
        }
    }
    else
    {
        auto type = TypeVisitor::assertType(c, node.target.get());

        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprLower::lower(c, node.params[i], type->args[i + 1]);
        }
    }
}

void ExprLower::visit(AddrOfNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
    node.setProperty("type", c.types.insert(TypeVisitor::assertType(c, node.expr.get())->addPointer()));
}

void ExprLower::visit(DerefNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);

    if(expectedType && expectedType->ref)
    {
        rn = new AddrOfNode(node.location(), cn);
        rn->setProperty("type", c.types.insert(TypeVisitor::assertType(c, &node)->addPointer()));
    }
}

void ExprLower::visit(AssignNode &node)
{
    node.target = ExprLower::lower(c, node.target);
    node.expr = ExprLower::lower(c, node.expr);
}

void ExprLower::visit(UnaryNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

void ExprLower::visit(BinaryNode &node)
{
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

void ExprLower::visit(LogicalNode &node)
{
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

void ExprLower::visit(PreIncDecNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

void ExprLower::visit(PostIncDecNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

void ExprLower::visit(CommaNode &node)
{
    node.first = ExprLower::lower(c, node.first);
    node.second = ExprLower::lower(c, node.second);
}

NodePtr ExprLower::lower(Context &c, NodePtr node, Type *expectedType)
{
    ExprLower el(c, node, expectedType);
    node->accept(el);

    return el.result() ? el.result() : node;
}

#include "ExprLower.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/CommaNode.h"
#include "nodes/InlineVarNode.h"
#include "nodes/TernaryNode.h"
#include "nodes/TypeCastNode.h"

#include "lower/FuncLower.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

namespace
{

template<typename T> void handleLiteral(T &node, Type *expectedType)
{
    if(expectedType && expectedType->ref)
    {
        if(!node.findProperty("temp_literal"))
        {
            throw Error(node.location(), "cannot reference literal - ", node.description());
        }
    }
}

template<typename T> NodePtr lowerCallNode(Context &c, T &node, NodePtr &cn, Type *type, Type *expectedType)
{
    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprLower::lower(c, node.params[i], type->args[i]);
    }

    if(type->returnType->ref)
    {
        if(!expectedType || !expectedType->ref)
        {
            NodePtr n(new DerefNode(node.location(), cn));
            n->setProperty("type", c.types.insert(type->returnType->removePointer()));

            return n;
        }
    }

    return { };
}

}

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

void ExprLower::visit(CharLiteralNode &node)
{
    handleLiteral(node, expectedType);
}

void ExprLower::visit(IntLiteralNode &node)
{
    handleLiteral(node, expectedType);
}

void ExprLower::visit(SizeLiteralNode &node)
{
    handleLiteral(node, expectedType);
}

void ExprLower::visit(BoolLiteralNode &node)
{
    handleLiteral(node, expectedType);
}

void ExprLower::visit(StringLiteralNode &node)
{
    handleLiteral(node, expectedType);
}

void ExprLower::visit(CallNode &node)
{
    auto type = TypeVisitor::assertType(c, node.target.get());

    if(auto n = lowerCallNode(c, node, cn, type, expectedType))
    {
        rn = n;
    }
}

void ExprLower::visit(ProxyCallNode &node)
{
    auto type = node.sym->property<Type*>("type");

    if(auto n = lowerCallNode(c, node, cn, type, expectedType))
    {
        rn = n;
    }
}

void ExprLower::visit(ConstructNode &node)
{
    if(node.type->primitive())
    {
        handleLiteral(node, expectedType);

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

        if(!expectedType || !expectedType->ref)
        {
            rn = new DerefNode(node.location(), cn);
            rn->setProperty("type", node.type);
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
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

void ExprLower::visit(InlineVarNode &node)
{
    Visitor::visit<FuncLower>(node.body.get(), c);
}

void ExprLower::visit(TernaryNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);

    node.left = ExprLower::lower(c, node.left, expectedType);
    node.right = ExprLower::lower(c, node.right, expectedType);
}

void ExprLower::visit(TypeCastNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

NodePtr ExprLower::lower(Context &c, NodePtr node, Type *expectedType)
{
    ExprLower el(c, node, expectedType);
    node->accept(el);

    return el.result() ? el.result() : node;
}

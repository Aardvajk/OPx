#include "ExprConvert.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
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

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeConvert.h"

#include "convert/CommonConvert.h"
#include "convert/FuncConvert.h"

#include "visitors/TypeVisitor.h"

namespace
{

void convertParams(Context &c, NodeList &params, Type *type)
{
    for(std::size_t i = 0; i < params.size(); ++i)
    {
        params[i] = CommonConvert::convert(c, params[i], type->args[i], TypeConvert::Permission::Implicit);
    }
}

}

ExprConvert::ExprConvert(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprConvert::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprConvert::convert(c, node.parent);
    }
}

void ExprConvert::visit(CallNode &node)
{
    node.target = ExprConvert::convert(c, node.target);

    for(auto &p: node.params)
    {
        p = ExprConvert::convert(c, p);
    }

    convertParams(c, node.params, TypeVisitor::assertType(c, node.target.get()));
}

void ExprConvert::visit(ProxyCallNode &node)
{
    for(auto &p: node.params)
    {
        p = ExprConvert::convert(c, p);
    }

    convertParams(c, node.params, node.sym->property<Type*>("type"));
}

void ExprConvert::visit(ConstructNode &node)
{
    for(auto &p: node.params)
    {
        p = ExprConvert::convert(c, p);
    }

    if(node.type->primitive() && !node.params.empty())
    {
        auto t = TypeVisitor::assertType(c, node.params[0].get());
        if(!t->primitive())
        {
            node.params[0] = CommonConvert::convert(c, node.params[0], node.type, TypeConvert::Permission::Explicit);
        }
    }
}

void ExprConvert::visit(AddrOfNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void ExprConvert::visit(DerefNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void ExprConvert::visit(AssignNode &node)
{
    node.target = ExprConvert::convert(c, node.target);
    node.expr = ExprConvert::convert(c, node.expr);

    node.expr = CommonConvert::convert(c, node.expr, TypeVisitor::assertType(c, node.target.get()), TypeConvert::Permission::Implicit);
}

void ExprConvert::visit(UnaryNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void ExprConvert::visit(BinaryNode &node)
{
    node.left = ExprConvert::convert(c, node.left);
    node.right = ExprConvert::convert(c, node.right);
}

void ExprConvert::visit(LogicalNode &node)
{
    node.left = ExprConvert::convert(c, node.left);
    node.right = ExprConvert::convert(c, node.right);

    node.left = CommonConvert::convert(c, node.left, c.types.boolType(), TypeConvert::Permission::Implicit);
    node.right = CommonConvert::convert(c, node.right, c.types.boolType(), TypeConvert::Permission::Implicit);
}

void ExprConvert::visit(PreIncDecNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void ExprConvert::visit(PostIncDecNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
}

void ExprConvert::visit(CommaNode &node)
{
    node.left = ExprConvert::convert(c, node.left);
    node.right = ExprConvert::convert(c, node.right);
}

void ExprConvert::visit(InlineVarNode &node)
{
    Visitor::visit<FuncConvert>(node.body.get(), c);
}

void ExprConvert::visit(TernaryNode &node)
{
    node.expr = ExprConvert::convert(c, node.expr);
    node.left = ExprConvert::convert(c, node.left);
    node.right = ExprConvert::convert(c, node.right);

    node.expr = CommonConvert::convert(c, node.expr, c.types.boolType(), TypeConvert::Permission::Implicit);
}

NodePtr ExprConvert::convert(Context &c, NodePtr &node, Type *expectedType)
{
    ExprConvert ec(c, node, expectedType);
    node->accept(ec);

    return ec.result() ? ec.result() : node;
}

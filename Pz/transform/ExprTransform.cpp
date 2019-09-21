#include "ExprTransform.h"

#include "application/Context.h"

#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

ExprTransform::ExprTransform(Context &c) : c(c)
{
}

void ExprTransform::visit(CallNode &node)
{
    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprTransform::transform(c, node.params[i]);
    }

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(type->method)
    {
        auto n = Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get());
        if(!n)
        {
            throw Error(node.target->location(), "cannot call method without object - ", node.target->description());
        }

        node.params.insert(node.params.begin(), n);
    }
}

void ExprTransform::visit(AddrOfNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(DerefNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(AssignNode &node)
{
    node.target = ExprTransform::transform(c, node.target);
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(BinaryNode &node)
{
    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

NodePtr ExprTransform::transform(Context &c, NodePtr &node)
{
    ExprTransform et(c);
    node->accept(et);

    return et.result() ? et.result() : node;
}

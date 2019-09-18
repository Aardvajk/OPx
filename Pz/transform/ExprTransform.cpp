#include "ExprTransform.h"

#include "application/Context.h"

#include "nodes/CallNode.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

ExprTransform::ExprTransform(Context &c) : c(c)
{
}

void ExprTransform::visit(CallNode &node)
{
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

NodePtr ExprTransform::transform(Context &c, NodePtr &node)
{
    ExprTransform et(c);
    node->accept(et);

    return et.result() ? et.result() : node;
}

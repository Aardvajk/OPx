#include "ExprTransformer.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/AssignNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"

#include "decorator/ExprDecorator.h"

ExprTransformer::ExprTransformer(Context &c) : c(c)
{
}

void ExprTransformer::visit(AssignNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    node.expr = ExprTransformer::transform(c, node.expr);

    auto cn = new CallNode(node.location(), new IdNode(node.location(), "operator="));
    rn = cn;

    cn->params.push_back(new AddrOfNode(node.target->location(), node.target));
    cn->params.push_back(node.expr);

    ExprDecorator::decorate(c, nullptr, *cn);
}

NodePtr ExprTransformer::transform(Context &c, NodePtr &node)
{
    ExprTransformer et(c);
    node->accept(et);

    auto r = et.result();

    return r ? r : node;
}

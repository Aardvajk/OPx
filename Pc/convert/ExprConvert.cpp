#include "ExprConvert.h"

#include "nodes/CallNode.h"

#include "types/Type.h"

#include "convert/CommonConvert.h"

#include "visitors/TypeVisitor.h"

ExprConvert::ExprConvert(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprConvert::visit(CallNode &node)
{
    auto t = TypeVisitor::assertType(c, node.target.get());

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = CommonConvert::convert(c, node.params[i], t->args[i]);
    }
}

NodePtr ExprConvert::convert(Context &c, NodePtr &node, Type *expectedType)
{
    ExprConvert ec(c, node, expectedType);
    node->accept(ec);

    return ec.result() ? ec.result() : node;
}

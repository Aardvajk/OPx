#include "ExprConvert.h"

#include "nodes/CallNode.h"

ExprConvert::ExprConvert(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprConvert::visit(CallNode &node)
{
    std::cout << "convert " << node.description() << "\n";
}

NodePtr ExprConvert::convert(Context &c, NodePtr &node, Type *expectedType)
{
    ExprConvert ec(c, node, expectedType);
    node->accept(ec);

    return ec.result() ? ec.result() : node;
}

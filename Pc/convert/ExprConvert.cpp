#include "ExprConvert.h"

ExprConvert::ExprConvert(Context &c, Type *expectedType) : c(c), expectedType(expectedType)
{
}

NodePtr ExprConvert::convert(Context &c, NodePtr &node, Type *expectedType)
{
    ExprConvert ec(c, expectedType);
    node->accept(ec);

    return ec.result() ? ec.result() : node;
}

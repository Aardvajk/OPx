#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/LiteralNodes.h"

ExprDecorator::ExprDecorator(Context &c) : c(c)
{
}

void ExprDecorator::visit(IntLiteralNode &node)
{
    node.setProperty("type", c.types.intType());
}

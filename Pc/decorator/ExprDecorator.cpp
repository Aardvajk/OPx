#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/CallNode.h"

ExprDecorator::ExprDecorator(Context &c) : c(c)
{
}

void ExprDecorator::visit(CallNode &node)
{
}

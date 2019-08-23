#include "ExprLower.h"

ExprLower::ExprLower(Context &c) : c(c)
{
}

NodePtr ExprLower::lower(Context &c, NodePtr &node)
{
    ExprLower el(c);
    node->accept(el);

    return el.result() ? el.result() : node;
}

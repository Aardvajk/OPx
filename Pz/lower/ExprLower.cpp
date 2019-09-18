#include "ExprLower.h"

#include "application/Context.h"

ExprLower::ExprLower(Context &c, NodePtr &cn) : c(c), cn(cn)
{
}

NodePtr ExprLower::lower(Context &c, NodePtr &node)
{
    ExprLower el(c, node);
    node->accept(el);

    return el.result() ? el.result() : node;
}

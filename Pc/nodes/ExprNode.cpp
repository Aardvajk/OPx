#include "ExprNode.h"

#include "visitors/Visitor.h"

void ExprNode::accept(Visitor &v)
{
    v.visit(*this);
}

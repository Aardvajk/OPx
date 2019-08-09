#include "IfNode.h"

#include "visitors/Visitor.h"

void IfNode::accept(Visitor &v)
{
    v.visit(*this);
}

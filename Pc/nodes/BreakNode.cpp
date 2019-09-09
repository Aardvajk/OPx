#include "BreakNode.h"

#include "visitors/Visitor.h"

void BreakNode::accept(Visitor &v)
{
    v.visit(*this);
}

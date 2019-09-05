#include "IncDecNode.h"

#include "visitors/Visitor.h"

void IncDecNode::accept(Visitor &v)
{
    v.visit(*this);
}

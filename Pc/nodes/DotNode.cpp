#include "DotNode.h"

#include "visitors/Visitor.h"

void DotNode::accept(Visitor &v)
{
    v.visit(*this);
}

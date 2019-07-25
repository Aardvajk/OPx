#include "ThisNode.h"

#include "visitors/Visitor.h"

void ThisNode::accept(Visitor &v)
{
    v.visit(*this);
}

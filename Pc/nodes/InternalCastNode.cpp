#include "InternalCastNode.h"

#include "visitors/Visitor.h"

void InternalCastNode::accept(Visitor &v)
{
    v.visit(*this);
}

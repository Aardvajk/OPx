#include "PrimitiveCastNode.h"

#include "visitors/Visitor.h"

void PrimitiveCastNode::accept(Visitor &v)
{
    v.visit(*this);
}

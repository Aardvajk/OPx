#include "OpEqNode.h"

#include "visitors/Visitor.h"

void OpEqNode::accept(Visitor &v)
{
    v.visit(*this);
}

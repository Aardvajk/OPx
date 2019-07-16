#include "AssignNode.h"

#include "visitors/Visitor.h"

void AssignNode::accept(Visitor &v)
{
    v.visit(*this);
}

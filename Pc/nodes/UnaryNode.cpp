#include "UnaryNode.h"

#include "visitors/Visitor.h"

void UnaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "WhileNode.h"

#include "visitors/Visitor.h"

void WhileNode::accept(Visitor &v)
{
    v.visit(*this);
}

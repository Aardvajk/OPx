#include "BinaryNode.h"

#include "visitors/Visitor.h"

void BinaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

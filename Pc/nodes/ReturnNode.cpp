#include "ReturnNode.h"

#include "visitors/Visitor.h"

void ReturnNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "SubscriptNode.h"

#include "visitors/Visitor.h"

void SubscriptNode::accept(Visitor &v)
{
    v.visit(*this);
}

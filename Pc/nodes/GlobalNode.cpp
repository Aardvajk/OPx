#include "GlobalNode.h"

#include "visitors/Visitor.h"

void GlobalNode::accept(Visitor &v)
{
    v.visit(*this);
}

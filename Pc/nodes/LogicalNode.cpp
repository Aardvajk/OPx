#include "LogicalNode.h"

#include "visitors/Visitor.h"

void LogicalNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *CallNode::classname() const
{
    return "call";
}

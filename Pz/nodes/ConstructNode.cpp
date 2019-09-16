#include "ConstructNode.h"

#include "visitors/Visitor.h"

void ConstructNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *ConstructNode::classname() const
{
    return "construct";
}

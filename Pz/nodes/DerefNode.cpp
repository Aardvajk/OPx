#include "DerefNode.h"

#include "visitors/Visitor.h"

void DerefNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *DerefNode::classname() const
{
    return "deref";
}

#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *TypeNode::classname() const
{
    return "TypeNode";
}

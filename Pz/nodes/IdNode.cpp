#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *IdNode::classname() const
{
    return "IdNode";
}

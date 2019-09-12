#include "VarNode.h"

#include "visitors/Visitor.h"

void VarNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *VarNode::classname() const
{
    return "VarNode";
}

#include "TypeNode.h"

#include "visitors/Visitor.h"

void TypeNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TypeNode::classname() const
{
    return "TypeNode";
}

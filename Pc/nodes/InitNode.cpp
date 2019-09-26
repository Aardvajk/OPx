#include "InitNode.h"

#include "visitors/Visitor.h"

void InitNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string InitNode::classname() const
{
    return "init";
}

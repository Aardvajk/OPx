#include "ForNode.h"

#include "visitors/Visitor.h"

void ForNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ForNode::classname() const
{
    return "for";
}

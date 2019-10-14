#include "TernaryNode.h"

#include "visitors/Visitor.h"

void TernaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TernaryNode::classname() const
{
    return "ternary";
}

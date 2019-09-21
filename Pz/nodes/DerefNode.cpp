#include "DerefNode.h"

#include "visitors/Visitor.h"

void DerefNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string DerefNode::classname() const
{
    return "deref";
}

#include "ThisNode.h"

#include "visitors/Visitor.h"

void ThisNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ThisNode::classname() const
{
    return "this";
}

Node *ThisNode::clone() const
{
    return cloneNode(this, new ThisNode(location()));
}

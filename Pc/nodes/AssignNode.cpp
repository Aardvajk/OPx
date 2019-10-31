#include "AssignNode.h"

#include "visitors/Visitor.h"

void AssignNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string AssignNode::classname() const
{
    return "assign";
}

Node *AssignNode::clone() const
{
    return cloneNode(this, new AssignNode(location(), safeClone(target), safeClone(expr)));
}

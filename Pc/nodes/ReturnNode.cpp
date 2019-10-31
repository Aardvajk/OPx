#include "ReturnNode.h"

#include "visitors/Visitor.h"

void ReturnNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ReturnNode::classname() const
{
    return "return";
}

Node *ReturnNode::clone() const
{
    return cloneNode(this, new ReturnNode(location(), safeClone(expr)));
}

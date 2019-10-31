#include "CommaNode.h"

#include "visitors/Visitor.h"

void CommaNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string CommaNode::classname() const
{
    return "comma";
}

Node *CommaNode::clone() const
{
    return cloneNode(this, new CommaNode(location(), safeClone(left), safeClone(right)));
}

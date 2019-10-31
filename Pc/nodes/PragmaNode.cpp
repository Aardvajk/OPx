#include "PragmaNode.h"

#include "visitors/Visitor.h"

void PragmaNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string PragmaNode::classname() const
{
    return "pragma";
}

Node *PragmaNode::clone() const
{
    return cloneNode(this, new PragmaNode(location(), type, arg));
}

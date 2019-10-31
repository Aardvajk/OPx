#include "TypeCastNode.h"

#include "visitors/Visitor.h"

void TypeCastNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TypeCastNode::classname() const
{
    return "typecast";
}

Node *TypeCastNode::clone() const
{
    return cloneNode(this, new TypeCastNode(location(), type, safeClone(expr)));
}

#include "UncheckedCastNode.h"

#include "visitors/Visitor.h"

void UncheckedCastNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string UncheckedCastNode::classname() const
{
    return "uncheckedcast";
}

Node *UncheckedCastNode::clone() const
{
    return cloneNode(this, new UncheckedCastNode(location(), safeClone(type), safeClone(expr)));
}

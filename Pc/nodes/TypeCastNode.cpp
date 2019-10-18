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

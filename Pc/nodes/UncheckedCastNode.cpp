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

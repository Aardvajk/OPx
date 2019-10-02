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

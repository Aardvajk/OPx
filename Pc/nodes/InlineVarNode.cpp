#include "InlineVarNode.h"

#include "visitors/Visitor.h"

void InlineVarNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string InlineVarNode::classname() const
{
    return "inlinevar";
}

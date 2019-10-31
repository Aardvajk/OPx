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

Node *InlineVarNode::clone() const
{
    return cloneNode(this, new InlineVarNode(location(), safeClone(body)));
}

#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IntLiteralNode::classname() const
{
    return "intliteral";
}

NodePtr IntLiteralNode::cloneDetail() const
{
    return new IntLiteralNode(location(), value);
}

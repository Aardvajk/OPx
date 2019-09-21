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

void BoolLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BoolLiteralNode::classname() const
{
    return "boolliteral";
}

void StringLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string StringLiteralNode::classname() const
{
    return "stringliteral";
}
#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *IntLiteralNode::classname() const
{
    return "intliteral";
}

void BoolLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *BoolLiteralNode::classname() const
{
    return "boolliteral";
}

void StringLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *StringLiteralNode::classname() const
{
    return "stringliteral";
}

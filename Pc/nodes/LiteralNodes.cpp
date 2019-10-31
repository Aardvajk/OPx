#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string CharLiteralNode::classname() const
{
    return "charliteral";
}

Node *CharLiteralNode::clone() const
{
    return cloneNode(this, new CharLiteralNode(location(), value));
}

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IntLiteralNode::classname() const
{
    return "intliteral";
}

Node *IntLiteralNode::clone() const
{
    return cloneNode(this, new IntLiteralNode(location(), value));
}

void SizeLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string SizeLiteralNode::classname() const
{
    return "sizeliteral";
}

Node *SizeLiteralNode::clone() const
{
    return cloneNode(this, new SizeLiteralNode(location(), value));
}

void BoolLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BoolLiteralNode::classname() const
{
    return "boolliteral";
}

Node *BoolLiteralNode::clone() const
{
    return cloneNode(this, new BoolLiteralNode(location(), value));
}

void StringLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string StringLiteralNode::classname() const
{
    return "stringliteral";
}

Node *StringLiteralNode::clone() const
{
    return cloneNode(this, new StringLiteralNode(location(), value));
}

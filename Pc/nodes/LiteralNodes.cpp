#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void NullLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

void BoolLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

void SizeLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

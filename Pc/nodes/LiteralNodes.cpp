#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

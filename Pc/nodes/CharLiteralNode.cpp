#include "CharLiteralNode.h"

#include "visitors/Visitor.h"

void CharLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

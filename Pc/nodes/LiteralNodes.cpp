#include "LiteralNodes.h"

#include "visitors/Visitor.h"

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

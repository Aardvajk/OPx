#include "NullLiteralNode.h"

#include "visitors/Visitor.h"

void NullLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

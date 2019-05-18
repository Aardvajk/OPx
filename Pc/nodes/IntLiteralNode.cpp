#include "IntLiteralNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void IntLiteralNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "ClassNode.h"

#include "visitors/Visitor.h"

void ClassNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "FuncDecNode.h"

#include "visitors/Visitor.h"

void FuncDecNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "FuncNode.h"

#include "visitors/Visitor.h"

void FuncNode::accept(Visitor &v)
{
    v.visit(*this);
}

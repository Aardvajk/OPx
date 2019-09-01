#include "PragmaNode.h"

#include "visitors/Visitor.h"

void PragmaNode::accept(Visitor &v)
{
    v.visit(*this);
}

#include "PragmaNode.h"

#include "visitors/Visitor.h"

void PragmaNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *PragmaNode::classname() const
{
    return "pragma";
}

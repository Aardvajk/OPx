#include "ScopeNode.h"

#include "visitors/Visitor.h"

void ScopeNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *ScopeNode::classname() const
{
    return "scope";
}

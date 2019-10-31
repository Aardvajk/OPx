#include "ScopeNode.h"

#include "visitors/Visitor.h"

void ScopeNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ScopeNode::classname() const
{
    return "scope";
}

Node *ScopeNode::clone() const
{
    return cloneNode(this, new ScopeNode(location(), safeClone(body)));
}

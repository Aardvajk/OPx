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

NodePtr ScopeNode::cloneDetail() const
{
    return copyDetails(this, new ScopeNode(location(), clonePtr(body)));
}

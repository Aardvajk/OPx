#include "NamespaceNode.h"

#include "visitors/Visitor.h"

void NamespaceNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string NamespaceNode::classname() const
{
    return "namespace";
}

NodePtr NamespaceNode::cloneDetail() const
{
    auto n = copyDetails(this, new NamespaceNode(location(), clonePtr(name)));
    n->body = clonePtr(body);

    return n;
}

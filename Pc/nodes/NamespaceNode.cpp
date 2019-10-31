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

Node *NamespaceNode::clone() const
{
    return cloneNode(this, new NamespaceNode(location(), safeClone(name), safeClone(body)));
}

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

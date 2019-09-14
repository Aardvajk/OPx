#include "NamespaceNode.h"

#include "visitors/Visitor.h"

void NamespaceNode::accept(Visitor &v)
{
    v.visit(*this);
}

const char *NamespaceNode::classname() const
{
    return "namespace";
}

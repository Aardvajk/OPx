#include "AddrOfNode.h"

#include "visitors/Visitor.h"

void AddrOfNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string AddrOfNode::classname() const
{
    return "addrof";
}

Node *AddrOfNode::clone() const
{
    return cloneNode(this, new AddrOfNode(location(), safeClone(expr)));
}

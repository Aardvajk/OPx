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

#include "ProxyCallNode.h"

#include "visitors/Visitor.h"

void ProxyCallNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ProxyCallNode::classname() const
{
    return "proxycall";
}

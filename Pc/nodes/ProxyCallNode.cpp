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

Node *ProxyCallNode::clone() const
{
    auto p = cloneNode(this, new ProxyCallNode(location(), sym, safeClone(thisNode)));

    p->params = listClone(params);

    return p;
}

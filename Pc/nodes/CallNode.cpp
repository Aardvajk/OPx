#include "CallNode.h"

#include "visitors/Visitor.h"

void CallNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string CallNode::classname() const
{
    return "call";
}

Node *CallNode::clone() const
{
    auto c = cloneNode(this, new CallNode(location(), target));
    c->params = listClone(params);

    return c;
}

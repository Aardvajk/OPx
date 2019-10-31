#include "SubscriptNode.h"

#include "visitors/Visitor.h"

void SubscriptNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string SubscriptNode::classname() const
{
    return "subscript";
}

Node *SubscriptNode::clone() const
{
    auto s = cloneNode(this, new SubscriptNode(location(), safeClone(target)));

    s->params = listClone(params);

    return s;
}

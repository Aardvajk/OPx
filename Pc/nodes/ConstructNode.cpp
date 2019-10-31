#include "ConstructNode.h"

#include "visitors/Visitor.h"

void ConstructNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ConstructNode::classname() const
{
    return "construct";
}

Node *ConstructNode::clone() const
{
    auto c = new ConstructNode(location(), type);
    c->params = listClone(params);

    return c;
}

#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IdNode::classname() const
{
    return "id";
}

NodePtr IdNode::cloneDetail() const
{
    auto i = copyDetails(this, new IdNode(location(), clonePtr(parent), name));
    NodePtr n(i);

    return n;
}

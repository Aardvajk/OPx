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

NodePtr IdNode::create(Location location, const std::vector<std::string> &names)
{
    NodePtr pn;
    for(std::size_t i = 0; i < names.size(); ++i)
    {
        pn = new IdNode(location, pn, names[i]);
    }

    return pn;
}

Node *IdNode::clone() const
{
    auto i = new IdNode(location(), safeClone(parent), name);

    i->special = special;
    i->op = op;
    i->arrow = arrow;

    return i;
}

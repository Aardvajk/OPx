#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
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

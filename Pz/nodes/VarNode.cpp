#include "VarNode.h"

#include "visitors/Visitor.h"

void VarNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string VarNode::classname() const
{
    return "var";
}

NodePtr VarNode::cloneDetail() const
{
    auto v = copyDetails(this, new VarNode(location(), clonePtr(name)));
    NodePtr n(v);

    v->type = clonePtr(type);

    return n;
}

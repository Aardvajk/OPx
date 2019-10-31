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

Node *VarNode::clone() const
{
    auto v = cloneNode(this, new VarNode(location(), safeClone(name)));

    v->type = safeClone(type);
    v->value = safeClone(value);

    return v;
}

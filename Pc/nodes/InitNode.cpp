#include "InitNode.h"

#include "visitors/Visitor.h"

void InitNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string InitNode::classname() const
{
    return "init";
}

Node *InitNode::clone() const
{
    auto i = cloneNode(this, new InitNode(location(), name));

    i->target = safeClone(target);
    i->params = listClone(params);

    return i;
}

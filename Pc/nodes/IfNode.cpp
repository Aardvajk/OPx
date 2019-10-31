#include "IfNode.h"

#include "visitors/Visitor.h"

void IfNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IfNode::classname() const
{
    return "if";
}

Node *IfNode::clone() const
{
    auto i = cloneNode(this, new IfNode(location()));

    i->expr = safeClone(expr);
    i->body = safeClone(body);
    i->elseBody = safeClone(elseBody);

    return i;
}

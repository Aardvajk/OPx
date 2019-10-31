#include "WhileNode.h"

#include "visitors/Visitor.h"

void WhileNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string WhileNode::classname() const
{
    return "while";
}

Node *WhileNode::clone() const
{
    auto w = cloneNode(this, new WhileNode(location()));

    w->expr = safeClone(expr);
    w->body = safeClone(body);

    return w;
}

#include "ForNode.h"

#include "visitors/Visitor.h"

void ForNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string ForNode::classname() const
{
    return "for";
}

Node *ForNode::clone() const
{
    auto f = cloneNode(this, new ForNode(location()));

    f->init = safeClone(init);
    f->cond = safeClone(cond);
    f->post = safeClone(post);
    f->body = safeClone(body);

    return f;
}

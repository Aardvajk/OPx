#include "TernaryNode.h"

#include "visitors/Visitor.h"

void TernaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string TernaryNode::classname() const
{
    return "ternary";
}

Node *TernaryNode::clone() const
{
    auto t = cloneNode(this, new TernaryNode(location(), safeClone(expr)));

    t->left = safeClone(left);
    t->right = safeClone(right);

    return t;
}

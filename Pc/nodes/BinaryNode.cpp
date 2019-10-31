#include "BinaryNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void BinaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BinaryNode::classname() const
{
    return pcx::str("binary", token.text());
}

Node *BinaryNode::clone() const
{
    return cloneNode(this, new BinaryNode(location(), token, safeClone(left), safeClone(right)));
}

#include "BinaryNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void BinaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string BinaryNode::classname() const
{
    return pcx::str("binary", type.text());
}

#include "UnaryNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void UnaryNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string UnaryNode::classname() const
{
    return pcx::str("unary", token.text());
}

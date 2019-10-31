#include "LogicalNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void LogicalNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string LogicalNode::classname() const
{
    return pcx::str("logical", token.text());
}

Node *LogicalNode::clone() const
{
    return cloneNode(this, new LogicalNode(location(), token, safeClone(left), safeClone(right)));
}

#include "DotNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void DotNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string DotNode::text() const
{
    return pcx::str(name, '.', child->text());
}

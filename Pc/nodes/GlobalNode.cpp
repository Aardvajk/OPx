#include "GlobalNode.h"

#include "visitors/Visitor.h"

#include <pcx/str.h>

void GlobalNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string GlobalNode::text() const
{
    return pcx::str('.', child->text());
}

#include "IdNode.h"

#include "visitors/Visitor.h"

void IdNode::accept(Visitor &v)
{
    v.visit(*this);
}

std::string IdNode::text() const
{
    return name;
}

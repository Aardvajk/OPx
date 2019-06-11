#include "IdNode.h"

#include "visitors/Visitor.h"

IdNode::IdNode(Location location, std::string value) : Node(location), value(std::move(value))
{
}

void IdNode::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

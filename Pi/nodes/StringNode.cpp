#include "StringNode.h"

#include "visitors/Visitor.h"

StringNode::StringNode(Location location, std::string value) : Node(location), value(std::move(value))
{
}

void StringNode::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

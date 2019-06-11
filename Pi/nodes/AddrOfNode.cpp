#include "AddrOfNode.h"

#include "visitors/Visitor.h"

AddrOfNode::AddrOfNode(Location location, NodePtr node) : Node(location), node(std::move(node))
{
}

void AddrOfNode::accept(Visitor &visitor)
{
    visitor.visit(*this);
}

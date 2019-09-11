#include "HasParent.h"

#include "nodes/IdNode.h"

HasParent::HasParent() : r(false)
{
}

void HasParent::visit(IdNode &node)
{
    r = node.parent;
}

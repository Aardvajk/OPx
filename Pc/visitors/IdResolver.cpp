#include "IdResolver.h"

#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

IdResolver::IdResolver()
{
}

void IdResolver::visit(IdNode &node)
{
    s = node.name;
}

void IdResolver::visit(DotNode &node)
{
    node.child->accept(*this);
}

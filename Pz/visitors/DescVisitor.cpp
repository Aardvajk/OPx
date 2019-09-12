#include "DescVisitor.h"

#include "nodes/BlockNode.h"

DescVisitor::DescVisitor()
{
}

void DescVisitor::visit(BlockNode &node)
{
    r += "(block)";
}

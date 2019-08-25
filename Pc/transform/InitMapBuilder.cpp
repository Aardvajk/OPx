#include "InitMapBuilder.h"

#include "nodes/InitNode.h"

InitMapBuilder::InitMapBuilder(std::unordered_map<std::string, NodePtr> &m, NodePtr &n) : m(m), n(n)
{
}

void InitMapBuilder::visit(InitNode &node)
{
    m[node.name] = n;
}

#include "Parser.h"

#include "nodes/BlockNode.h"

NodePtr Parser::build(Context &c)
{
    auto b = new BlockNode({ });
    NodePtr n(b);

    return n;
}

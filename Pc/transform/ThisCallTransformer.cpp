#include "ThisCallTransformer.h"

#include "framework/Error.h"

#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

ThisCallTransformer::ThisCallTransformer(Context &c) : c(c)
{
}

void ThisCallTransformer::visit(IdNode &node)
{
    if(!node.parent)
    {
        throw Error(node.location(), "temp error, no id parent for this generator");
    }

    auto a = new AddrOfNode(node.location());
    rn = a;

    a->expr = node.parent;
}

#include "ArraySizeVisitor.h"

#include "framework/Error.h"

#include "nodes/LiteralNodes.h"

#include "visitors/NameVisitors.h"

ArraySizeVisitor::ArraySizeVisitor()
{
}

void ArraySizeVisitor::visit(IntLiteralNode &node)
{
    r = static_cast<std::size_t>(node.value);
}

void ArraySizeVisitor::visit(SizeLiteralNode &node)
{
    r = node.value;
}

std::size_t ArraySizeVisitor::value(Node &node)
{
    ArraySizeVisitor av;
    node.accept(av);

    if(!av.result())
    {
        throw Error(node.location(), "constant expected - ", NameVisitors::prettyName(&node));
    }

    return *av.result();
}

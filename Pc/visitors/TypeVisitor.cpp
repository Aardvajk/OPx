#include "TypeVisitor.h"

#include "nodes/VarNode.h"

#include "types/TypeBuilder.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(VarNode &node)
{
    r = TypeBuilder::type(c, node.type.get());
}

const Type *TypeVisitor::type(Context &c, Node *node)
{
    TypeVisitor tv(c);
    node->accept(tv);

    return tv.result();
}

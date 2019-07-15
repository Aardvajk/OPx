#include "TypeVisitor.h"

#include "application/Context.h"

#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(VarNode &node)
{
    r = TypeBuilder::type(c, node.type.get());
}

void TypeVisitor::visit(CharLiteralNode &node)
{
    r = c.types.insert(Type(0, c.tree.root()->child("std")->child("char")));
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    r = c.types.insert(Type(0, c.tree.root()->child("std")->child("int")));
}

const Type *TypeVisitor::type(Context &c, Node *node)
{
    TypeVisitor tv(c);
    node->accept(tv);

    return tv.result();
}

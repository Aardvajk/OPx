#include "TypeVisitor.h"

#include "application/Context.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), type(nullptr)
{
}

void TypeVisitor::visit(CharLiteralNode &node)
{
    type = c.types.insert(Type(c.tree.root()->child("std")->child("char")));
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    type = c.types.insert(Type(c.tree.root()->child("std")->child("int")));
}

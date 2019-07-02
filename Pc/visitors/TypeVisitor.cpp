#include "TypeVisitor.h"

#include "application/Context.h"

#include "nodes/CallNode.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), type(nullptr)
{
}

void TypeVisitor::visit(NullLiteralNode &node)
{
    type = c.types.insert(Type(c.tree.root()->child("std")->child("null")));
}

void TypeVisitor::visit(CharLiteralNode &node)
{
    type = c.types.insert(Type(c.tree.root()->child("std")->child("char")));
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    type = c.types.insert(Type(c.tree.root()->child("std")->child("int")));
}

void TypeVisitor::visit(CallNode &node)
{
    type = c.types.insert(node.property("sym").to<const Sym*>()->property("type").to<const Type*>()->returnType.get());
}

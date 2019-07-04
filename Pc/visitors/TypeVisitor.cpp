#include "TypeVisitor.h"

#include "application/Context.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/CallNode.h"

namespace
{

const Type *fromId(Node &node)
{
    return node.property("syms").to<std::vector<SymResult> >().front().sym->property("type").to<const Type*>();
}

}

TypeVisitor::TypeVisitor(Context &c) : c(c), type(nullptr)
{
}

void TypeVisitor::visit(GlobalNode &node)
{
    type = fromId(node);
}

void TypeVisitor::visit(IdNode &node)
{
    type = fromId(node);
}

void TypeVisitor::visit(DotNode &node)
{
    type = fromId(node);
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

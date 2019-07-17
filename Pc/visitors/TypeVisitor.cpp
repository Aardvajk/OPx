#include "TypeVisitor.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(IdNode &node)
{
    r = node.property<const Sym*>("sym")->property<Type*>("type");
}

void TypeVisitor::visit(VarNode &node)
{
    r = TypeBuilder::type(c, node.type.get());
}

void TypeVisitor::visit(NullLiteralNode &node)
{
    r = c.types.nullType();
}

void TypeVisitor::visit(CharLiteralNode &node)
{
    r = c.types.insert(Type::makePrimary(0, c.tree.root()->child("std")->child("char")));
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    r = c.types.insert(Type::makePrimary(0, c.tree.root()->child("std")->child("int")));
}

void TypeVisitor::visit(CallNode &node)
{
    r = TypeVisitor::type(c, node.target.get())->returnType;
}

void TypeVisitor::visit(AddrOfNode &node)
{
    auto t = *(TypeVisitor::type(c, node.expr.get()));
    ++t.ptr;

    r = c.types.insert(t);
}

Type *TypeVisitor::type(Context &c, Node *node)
{
    TypeVisitor tv(c);
    node->accept(tv);

    return tv.result();
}

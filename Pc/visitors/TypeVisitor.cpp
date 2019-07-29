#include "TypeVisitor.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"

#include "visitors/NameVisitors.h"

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

void TypeVisitor::visit(BoolLiteralNode &node)
{
    r = c.types.insert(Type::makePrimary(0, c.tree.root()->child("std")->child("bool")));
}

void TypeVisitor::visit(SizeLiteralNode &node)
{
    r = c.types.insert(Type::makePrimary(0, c.tree.root()->child("std")->child("size")));
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

void TypeVisitor::visit(AssignNode &node)
{
    node.target->accept(*this);
}

void TypeVisitor::visit(ThisNode &node)
{
    auto f = c.tree.current()->container();

    auto p = f->parent();
    if(p->type() == Sym::Type::Class)
    {
        r = c.types.insert(Type::makePrimary(1, p));
    }
    else
    {
        throw Error(node.location(), "this cannot be used in a static function");
    }
}

void TypeVisitor::visit(DerefNode &node)
{
    auto t = *(TypeVisitor::type(c, node.expr.get()));

    if(!t.ptr)
    {
        throw Error(node.expr->location(), "pointer expected - ", NameVisitors::prettyName(node.expr.get()));
    }

    --t.ptr;
    r = c.types.insert(t);
}

void TypeVisitor::visit(BinaryNode &node)
{
    node.left->accept(*this);
}

void TypeVisitor::visit(SubscriptNode &node)
{
    auto t = *(TypeVisitor::type(c, node.target.get()));

    if(!t.ptr)
    {
        throw Error(node.expr->location(), "pointer expected - ", NameVisitors::prettyName(node.target.get()));
    }

    --t.ptr;
    r = c.types.insert(t);
}

Type *TypeVisitor::type(Context &c, Node *node)
{
    TypeVisitor tv(c);
    node->accept(tv);

    return tv.result();
}

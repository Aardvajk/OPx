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
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/PrimitiveCastNode.h"
#include "nodes/IncDecNode.h"

#include "visitors/NameVisitors.h"

#include "decorator/CommonDecorator.h"
#include "decorator/ExprDecorator.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(IdNode &node)
{
    if(!node.getProperty("sym"))
    {
        ExprDecorator::decorate(c, nullptr, node);
    }

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

void TypeVisitor::visit(StringLiteralNode &node)
{
    auto t = Type::makePrimary(1, c.tree.root()->child("std")->child("char"));
    t.sub = node.value.size() + 1;

    r = c.types.insert(t);
}

void TypeVisitor::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());

    if(t->function())
    {
        r = t->returnType;
    }
    else
    {
        r = t;
    }
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
        auto t = Type::makePrimary(0, p);
        t.ref = true;

        if(c.refsLowered)
        {
            ++t.ptr;
        }

        r = c.types.insert(t);
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

void TypeVisitor::visit(UnaryNode &node)
{
    if(!TypeVisitor::type(c, node.expr.get())->primitive())
    {
        NodePtr nn(new IdNode(node.location(), { }, pcx::str("operator", Operators::toString(node.op))));

        auto t = Type::makeFunction(0, c.types.nullType());

        t.args.push_back(TypeVisitor::type(c, node.expr.get()));

        r = CommonDecorator::searchCallableByType(c, *nn, &t)->property<const Type*>("type")->returnType;
    }
    else
    {
        switch(node.op)
        {
            case Operators::Type::Not:
            case Operators::Type::Neg: node.expr->accept(*this); break;

            default: throw Error("internal error - operator not supported");
        }
    }
}

void TypeVisitor::visit(BinaryNode &node)
{
    if(!TypeVisitor::type(c, node.left.get())->primitive() || !TypeVisitor::type(c, node.right.get())->primitive())
    {
        NodePtr nn(new IdNode(node.location(), { }, pcx::str("operator", Operators::toString(node.op))));

        auto t = Type::makeFunction(0, c.types.nullType());

        t.args.push_back(TypeVisitor::type(c, node.left.get()));
        t.args.push_back(TypeVisitor::type(c, node.right.get()));

        r = CommonDecorator::searchCallableByType(c, *nn, &t)->property<const Type*>("type")->returnType;
    }
    else
    {
        switch(node.op)
        {
            case Operators::Type::Add:
            case Operators::Type::Sub:
            case Operators::Type::Mul:
            case Operators::Type::Div:
            case Operators::Type::Mod: node.left->accept(*this); break;

            case Operators::Type::Eq:
            case Operators::Type::Neq:
            case Operators::Type::Lt:
            case Operators::Type::LtEq:
            case Operators::Type::Gt:
            case Operators::Type::GtEq: r = c.types.boolType(); break;

            default: throw Error("internal error - operator not supported");
        }
    }
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

void TypeVisitor::visit(PrimitiveCastNode &node)
{
    r = node.type;
}

void TypeVisitor::visit(LogicalNode &node)
{
    r = c.types.boolType();
}

void TypeVisitor::visit(IncDecNode &node)
{
    node.target->accept(*this);
}

Type *TypeVisitor::type(Context &c, Node *node)
{
    TypeVisitor tv(c);
    node->accept(tv);

    if(!tv.result())
    {
        throw Error(node->location(), "internal error, type lookup failed - ", NameVisitors::prettyName(node));
    }

    return tv.result();
}


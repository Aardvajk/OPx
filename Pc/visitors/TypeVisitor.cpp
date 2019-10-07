#include "TypeVisitor.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/InitNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/CommaNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(IdNode &node)
{
    if(auto t = node.property<Sym*>("sym")->findProperty("type"))
    {
        r = t.to<Type*>();
    }
}

void TypeVisitor::visit(TypeNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(VarNode &node)
{
    if(node.type)
    {
        r = node.type->property<Type*>("type");
    }
    else
    {
        r = node.property<Sym*>("sym")->property<Type*>("type");
    }
}

void TypeVisitor::visit(CharLiteralNode &node)
{
    r = c.types.charType();
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    r = c.types.intType();
}

void TypeVisitor::visit(BoolLiteralNode &node)
{
    r = c.types.boolType();
}

void TypeVisitor::visit(StringLiteralNode &node)
{
    r = c.types.insert(c.types.charType()->addPointer());
}

void TypeVisitor::visit(CallNode &node)
{
    auto t = Visitor::query<TypeVisitor, Type*>(node.target.get(), c);
    r = t->returnType;
}

void TypeVisitor::visit(ProxyCallNode &node)
{
    r = node.sym->property<Type*>("type")->returnType;
}

void TypeVisitor::visit(ConstructNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(AddrOfNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(DerefNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(ThisNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(AssignNode &node)
{
    node.target->accept(*this);
}

void TypeVisitor::visit(UnaryNode &node)
{
    node.expr->accept(*this);
}

void TypeVisitor::visit(BinaryNode &node)
{
    switch(node.token.type())
    {
        case Token::Type::Add:
        case Token::Type::Sub:
        case Token::Type::Star:
        case Token::Type::Div:
        case Token::Type::Mod:
        case Token::Type::AddEq:
        case Token::Type::SubEq:
        case Token::Type::MulEq:
        case Token::Type::DivEq:
        case Token::Type::ModEq: node.left->accept(*this); break;

        case Token::Type::Eq:
        case Token::Type::Neq:
        case Token::Type::Lt:
        case Token::Type::LtEq:
        case Token::Type::Gt:
        case Token::Type::GtEq: r = c.types.boolType(); break;

        default: throw Error(node.location(), "operator not supported - ", node.token.text());
    }
}

void TypeVisitor::visit(LogicalNode &node)
{
    r = c.types.boolType();
}

void TypeVisitor::visit(InitNode &node)
{
    r = node.property<Sym*>("sym")->property<Type*>("type");
}

void TypeVisitor::visit(PreIncDecNode &node)
{
    node.expr->accept(*this);
}

void TypeVisitor::visit(PostIncDecNode &node)
{
    node.expr->accept(*this);
}

void TypeVisitor::visit(CommaNode &node)
{
    node.second->accept(*this);
}

Type *TypeVisitor::queryType(Context &c, Node *node)
{
    return Visitor::query<TypeVisitor, Type*>(node, c);
}

Type *TypeVisitor::assertType(Context &c, Node *node)
{
    auto r = queryType(c, node);
    if(!r)
    {
        throw Error(node->location(), "type visitor failed - ", node->classname(), "node ", node->description());
    }

    return r;
}

#include "NameVisitors.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "scanner/Lexer.h"

#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/TextNode.h"

#include "types/Type.h"
#include "types/TypeBuilder.h"

NameVisitors::IsNameSimple::IsNameSimple() : r(false)
{
}

void NameVisitors::IsNameSimple::visit(IdNode &node)
{
    if(!node.parent)
    {
        r = true;
    }
}

NameVisitors::LastIdOfName::LastIdOfName()
{
}

void NameVisitors::LastIdOfName::visit(IdNode &node)
{
    r = node.name;
}

NameVisitors::SpecialName::SpecialName() : r(Token::Type::Invalid)
{
}

void NameVisitors::SpecialName::visit(IdNode &node)
{
    r = node.special;
}

NameVisitors::ResolveOpName::ResolveOpName(Context &c) : c(c), r(nullptr)
{
}

void NameVisitors::ResolveOpName::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent->accept(*this);
    }

    if(node.op)
    {
        r = Visitor::query<ResolveOpType, Type*>(node.op.get(), c, node);
    }
}

NameVisitors::ResolveOpType::ResolveOpType(Context &c, IdNode &id) : c(c), id(id), r(nullptr)
{
}

void NameVisitors::ResolveOpType::visit(TypeNode &node)
{
    auto t = Visitor::query<TypeBuilder, Type*>(&node, c);

    id.name += " " + t->text();
    id.op = { };

    r = t;
}

void NameVisitors::ResolveOpType::visit(TextNode &node)
{
    id.name += node.description();
    id.op = { };
}

std::string NameVisitors::assertSimpleName(Context &c, Node *node)
{
    if(!Visitor::query<IsNameSimple, bool>(node))
    {
        throw Error(node->location(), "simple name expected - ", node->description());
    }

    return Visitor::query<LastIdOfName, std::string>(node);
}

std::string NameVisitors::assertUniqueName(Context &c, Node *node)
{
    auto n = Visitor::query<LastIdOfName, std::string>(node);
    if(c.tree.current()->child(n))
    {
        throw Error(node->location(), "already defined - ", n);
    }

    return n;
}

std::string NameVisitors::assertSimpleUniqueName(Context &c, Node *node)
{
    assertSimpleName(c, node);
    return assertUniqueName(c, node);
}

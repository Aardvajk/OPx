#include "NameVisitors.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "scanner/Lexer.h"

#include "nodes/IdNode.h"
#include "nodes/TextNode.h"

#include "visitors/SymFinder.h"

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

NameVisitors::ResolveOpName::ResolveOpName(Context &c) : c(c)
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
        Visitor::visit<ResolveOpType>(node.op.get(), c, node);
    }
}

NameVisitors::ResolveOpType::ResolveOpType(Context &c, IdNode &id) : c(c), id(id)
{
}

void NameVisitors::ResolveOpType::visit(IdNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous - ", node.description());
    }

    if(sv.empty() || sv.front()->type() != Sym::Type::Class)
    {
        throw Error(node.location(), "type expected - ", node.description());
    }

    id.name += pcx::str(" ", sv.front()->fullname());
    id.op = { };
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

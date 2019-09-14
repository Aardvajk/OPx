#include "NameVisitors.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "scanner/Lexer.h"

#include "nodes/IdNode.h"

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

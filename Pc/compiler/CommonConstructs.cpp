#include "CommonConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/ScopeNode.h"

#include "compiler/Compiler.h"

#include <algorithm>

namespace
{

NodePtr nameImpl(Context &c, bool allowOperator, NodePtr parent, bool get)
{
    std::string name;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwOperator && allowOperator)
    {
        static const std::vector<Token::Type> operators =
        {
            Token::Type::Assign
        };

        auto op = c.scanner.next(true);
        if(std::find(operators.begin(), operators.end(), op.type()) == operators.end())
        {
            throw Error(op.location(), "invalid operator - ", op.text());
        }

        name = tok.text() + op.text();
    }
    else
    {
        tok = c.scanner.match(Token::Type::Id, false);
        name = tok.text();
    }

    auto n = new IdNode(tok.location(), parent, name);
    NodePtr nn(n);

    c.scanner.next(true);
    while(c.scanner.token().type() == Token::Type::Dot)
    {
        nn = nameImpl(c, allowOperator, nn, true);
    }

    return nn;
}

}

NodePtr CommonConstructs::name(Context &c, bool get)
{
    return nameImpl(c, false, { }, get);
}

NodePtr CommonConstructs::extendedName(Context &c, bool get)
{
    return nameImpl(c, true, { }, get);
}

NodePtr CommonConstructs::blockContents(Context &c, Location location, bool get)
{
    auto block = new BlockNode(location);
    NodePtr nn(block);

    c.scanner.match(Token::Type::LeftBrace, get);

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::RightBrace)
    {
        Compiler::construct(c, block, false);
    }

    c.scanner.next(true);

    return nn;
}

NodePtr CommonConstructs::scopeContents(Context &c, Location location, bool get)
{
    auto scope = new ScopeNode(location);
    NodePtr nn(scope);

    auto block = new BlockNode(location);
    scope->body = block;

    auto tok = c.scanner.next(true);

    if(tok.type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            Compiler::construct(c, block, false);
        }

        c.scanner.next(true);
    }
    else
    {
        Compiler::construct(c, block, false);
    }

    return nn;
}

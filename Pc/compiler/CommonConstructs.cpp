#include "CommonConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/ScopeNode.h"

#include "compiler/Compiler.h"

#include "operators/Operators.h"

namespace
{

NodePtr nameImpl(Context &c, bool allowExtensions, NodePtr parent, bool get)
{
    std::string name;
    Token::Type spec = Token::Type::Invalid;

    auto tok = c.scanner.next(get);
    if(tok.type() == Token::Type::RwOperator && allowExtensions)
    {
        auto op = Operators::scan(c.scanner, true);
        name = tok.text() + op.text();
    }
    else if((tok.type() == Token::Type::RwNew || tok.type() == Token::Type::RwDelete) && allowExtensions)
    {
        name = tok.text();
        spec = tok.type();
    }
    else
    {
        tok = c.scanner.match(Token::Type::Id, false);
        name = tok.text();
    }

    auto n = new IdNode(tok.location(), parent, name);
    n->special = spec;

    NodePtr nn(n);

    c.scanner.next(true);

    while(c.scanner.token().type() == Token::Type::Dot)
    {
        if(spec != Token::Type::Invalid)
        {
            throw Error(c.scanner.token().location(), "invalid syntax");
        }

        nn = nameImpl(c, allowExtensions, nn, true);
    }

    return nn;
}

NodePtr scopeContentsImp(Context &c, Location location, bool loop, bool get)
{
    auto scope = new ScopeNode(location, loop);
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

NodePtr CommonConstructs::funcContents(Context &c, Location location, bool get)
{
    auto scope = new ScopeNode(location);
    NodePtr nn(scope);

    auto block = new BlockNode(location);
    scope->body = block;

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
    return scopeContentsImp(c, location, false, get);
}

NodePtr CommonConstructs::loopScopeContents(Context &c, Location location, bool get)
{
    return scopeContentsImp(c, location, true, get);
}

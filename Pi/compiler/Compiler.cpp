#include "Compiler.h"

#include "error/Error.h"

#include "application/Context.h"

#include "compiler/InstructionConstruct.h"

#include <pcx/lexical_cast.h>

namespace
{

void commonSizeConstruct(Context &c, Sym *sym, bool get)
{
    c.scanner.next(get);
    if(c.scanner.token().type() == Token::Type::Colon)
    {
        auto size = c.scanner.match(Token::Type::IntLiteral, true);
        sym->properties["size"] = pcx::lexical_cast<std::size_t>(size.text());

        c.scanner.next(true);
    }
}

void commonVarConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::StringLiteral, get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Var, id.text()));
    commonSizeConstruct(c, sym, true);
}

void varConstruct(Context &c, bool get)
{
    commonVarConstruct(c, get);

    c.scanner.consume(Token::Type::Semicolon, false);
}

void argConstruct(Context &c, bool get)
{
    commonVarConstruct(c, get);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        argConstruct(c, true);
    }
}

void funcConstruct(Context &c, bool get)
{
    auto id = c.scanner.match(Token::Type::StringLiteral, get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Func, id.text()));

    c.syms.push();

    c.scanner.consume(Token::Type::LeftParen, true);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        argConstruct(c, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    commonSizeConstruct(c, sym, false);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            InstructionConstruct::entity(c, false);
        }

        c.scanner.consume(Token::Type::RightBrace, false);
    }
    else
    {
        c.scanner.consume(Token::Type::Semicolon, false);
    }

    c.syms.pop();
}

void construct(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::RwVar: varConstruct(c, true); break;
        case Token::Type::RwFunc: funcConstruct(c, true); break;

        default: throw Error(tok.location(), "construct expected - ", tok.text());
    }
}

}

void compile(Context &c)
{
    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        construct(c, false);
    }
}

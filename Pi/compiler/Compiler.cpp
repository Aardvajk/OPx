#include "Compiler.h"

#include "framework/Error.h"

#include "common/OpCode.h"

#include "application/Context.h"

#include "compiler/Instructions.h"

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

void varConstruct(Context &c, Sym::Type type, bool get)
{
    auto id = c.scanner.match(Token::Type::StringLiteral, get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(type, id.text()));
    commonSizeConstruct(c, sym, true);

    c.scanner.consume(Token::Type::Semicolon, false);
}

void funcConstruct(Context &c, bool get)
{
    static int number = 1000;

    auto id = c.scanner.match(Token::Type::StringLiteral, get);
    c.assertUnique(id.location(), id.text());

    auto sym = c.syms.add(new Sym(Sym::Type::Func, id.text()));

    c.syms.push();
    c.funcs.emplace_back(sym, c.strings.insert(id.text()));

    commonSizeConstruct(c, sym, true);

    if(c.scanner.token().type() == Token::Type::LeftBrace)
    {
        c.scanner.next(true);
        while(c.scanner.token().type() == Token::Type::RwVar)
        {
            varConstruct(c, Sym::Type::Arg, true);
        }

        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            Instructions::entity(c, false);
        }

        c.scanner.consume(Token::Type::RightBrace, false);

        c.funcs.back().bytes << OpCode::Op::Int << number++;
        c.funcs.back().bytes << OpCode::Op::Ret << std::size_t(0);
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
        case Token::Type::RwVar: varConstruct(c, Sym::Type::Var, true); break;
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

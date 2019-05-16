#include "TestConstructs.h"

#include "application/Context.h"
#include "application/Error.h"

#include "compiler/Compiler.h"
#include "compiler/CommonConstructs.h"

#include "scanner/Lexer.h"

#include "nodes/BlockNode.h"

#include "visitors/AstPrinter.h"
#include "visitors/SymFinder.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

std::string formatResult(const SymFinder::Result &r)
{
    return pcx::str(r.accessible ? "" : "private ", r.sym->fullname());
}

}

void TestConstructs::lookup(Context &c, bool get)
{
    c.scanner.match(Token::Type::LeftParen, get);

    auto nn = CommonConstructs::name(c, true);

    c.scanner.consume(Token::Type::Comma, false);

    auto expected = Lexer::decodeString(c.scanner.match(Token::Type::StringLiteral, false).text());

    SymFinder sf(SymFinder::Policy::Full, c.tree.root(), c.tree.current());
    nn->accept(sf);

    auto result = pcx::join_str(sf.result(), ", ", formatResult);

    if(result != expected)
    {
        throw Error(nn->location(), "lookup failed - expected: ", Lexer::encodeString(expected), " - result: ", Lexer::encodeString(result));
    }

    c.scanner.consume(Token::Type::RightParen, true);
    c.scanner.consume(Token::Type::Semicolon, false);
}

void TestConstructs::triggerError(Context &c, BlockNode *block, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto expected = Lexer::decodeString(c.scanner.match(Token::Type::StringLiteral, true).text());

    c.scanner.match(Token::Type::RightParen, true);

    try
    {
        auto n = c.scanner.token().location();

        auto scope = new BlockNode(n);
        block->nodes.push_back(scope);

        auto s = c.tree.current()->add(new Sym(Sym::Type::Scope, { }, n, { }));
        auto g = c.tree.open(s);

        c.scanner.consume(Token::Type::LeftBrace, true);
        while(c.scanner.token().type() != Token::Type::RightBrace)
        {
            Compiler::construct(c, block, false);
        }

        c.scanner.consume(Token::Type::RightBrace, false);
    }

    catch(const Error &error)
    {
        auto result = error.what();
        if(result != expected)
        {
            throw Error(tok.location(), "trigger_error failed - expected: ", Lexer::encodeString(expected), " - result: ", Lexer::encodeString(result));
        }

        c.scanner.recover(1, true);
        return;
    }

    throw Error(tok.location(), "trigger_error failed - expected: ", Lexer::encodeString(expected), " - result: no error");
}

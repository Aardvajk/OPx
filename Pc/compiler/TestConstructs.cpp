#include "TestConstructs.h"

#include "application/Context.h"
#include "application/Error.h"

#include "compiler/CommonConstructs.h"

#include "scanner/Lexer.h"

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

    std::cout << "lookup passed: " << Lexer::encodeString(expected) << "\n";

    c.scanner.consume(Token::Type::RightParen, true);
    c.scanner.consume(Token::Type::Semicolon, false);
}

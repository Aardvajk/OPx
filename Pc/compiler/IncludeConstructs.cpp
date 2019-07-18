#include "IncludeConstructs.h"

#include "application/Context.h"

#include "compiler/Compiler.h"

void IncludeConstructs::entity(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwInclude, get);

    auto path = c.scanner.match(Token::Type::StringLiteral, true);

    c.open(path.text());

    c.scanner.next(true);
    while(c.scanner.token().type() != Token::Type::Eof)
    {
        Compiler::construct(c, block, false);
    }

    c.scanner.pop();
    c.scanner.consume(Token::Type::Semicolon, true);
}

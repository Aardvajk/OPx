#include "PragmaParser.h"

#include "framework/Error.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/PragmaNode.h"

void PragmaParser::build(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::LeftParen, get);

    auto tok = c.scanner.match(Token::Type::Id, true);

    if(c.containers.back() != Sym::Type::Namespace)
    {
        throw Error(tok.location(), "invalid pragma");
    }

    auto type = Pragmas::fromString(tok.text());
    if(type == Pragmas::Type::Invalid)
    {
        throw Error(tok.location(), "unknown pragma - ", tok.text());
    }

    auto n = new PragmaNode(tok.location(), type);
    block->push_back(n);

    tok = c.scanner.next(true);
    if(tok.type() == Token::Type::Comma)
    {
        n->arg = c.scanner.match(Token::Type::StringLiteral, true).text();
        c.scanner.next(true);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    c.scanner.consume(Token::Type::Semicolon, false);
}

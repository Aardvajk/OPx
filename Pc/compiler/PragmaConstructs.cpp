#include "PragmaConstructs.h"

#include "application/Context.h"
#include "application/Pragmas.h"

#include "nodes/BlockNode.h"
#include "nodes/PragmaNode.h"

void PragmaConstructs::entity(Context &c, BlockNode *block, bool get)
{
    c.scanner.match(Token::Type::RwPragma, get);
    c.scanner.match(Token::Type::LeftParen, true);

    auto tok = c.scanner.match(Token::Type::Id, true);

    auto cmd = Pragmas::fromString(tok.text());
    if(cmd == Pragmas::Type::Invalid)
    {
        throw Error(tok.location(), "unknown pragma - ", tok.text());
    }

    if(cmd == Pragmas::Type::Push)
    {
        ++c.pragmaPushes;
    }
    else if(cmd == Pragmas::Type::Pop)
    {
        if(!c.pragmaPushes)
        {
            throw Error(tok.location(), "pragma pop without previous push");
        }

        --c.pragmaPushes;
    }

    bool takesArg = Pragmas::takesArgument(cmd);

    auto n = new PragmaNode(tok.location(), cmd);
    block->push_back(n);

    tok = c.scanner.next(true);
    if(c.scanner.token().type() == Token::Type::Comma)
    {
        n->arg = c.scanner.match(Token::Type::StringLiteral, true).text();

        if(!takesArg)
        {
            throw Error(tok.location(), "unexpected argument to pragma - ", Pragmas::toString(cmd));
        }

        c.scanner.next(true);
    }
    else if(takesArg)
    {
        throw Error(tok.location(), "pragma missing argument - ", Pragmas::toString(cmd));
    }

    c.scanner.consume(Token::Type::RightParen, false);
    c.scanner.consume(Token::Type::Semicolon, false);
}


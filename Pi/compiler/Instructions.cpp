#include "Instructions.h"

#include "error/Error.h"

#include "application/Context.h"

#include <pcx/lexical_cast.h>

namespace
{

void retInstruction(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::IntLiteral, get);
    c.curr() << char(0) << pcx::lexical_cast<std::size_t>(tok.text());

    c.scanner.consume(Token::Type::Semicolon, true);
}

}

void Instructions::entity(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.text() == "ret")
    {
        retInstruction(c, true);
    }
    else
    {
        throw Error(tok.location(), "instruction expeced - ", tok.text());
    }
}

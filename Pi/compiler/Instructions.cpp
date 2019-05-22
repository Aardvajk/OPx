#include "Instructions.h"

#include "error/Error.h"

#include "application/Context.h"

#include <pcx/lexical_cast.h>

void Instructions::entity(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.text() == "foo")
    {
        c.scanner.consume(Token::Type::Semicolon, true);
        return;
    }

    throw Error(tok.location(), "instruction expeced - ", tok.text());
}

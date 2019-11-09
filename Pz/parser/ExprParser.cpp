#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/LiteralNodes.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    NodePtr n;

    switch(tok.type())
    {
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

        default: throw Error(tok.location(), "primary expected - ", tok.text());
    }
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return primary(c, get);
}

#include "Expr.h"

#include "error/Error.h"

#include "application/Context.h"

#include "nodes/IntLiteralNode.h"

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

        default: break;
    }

    throw Error(tok.location(), "primary expected - ", tok.text());
}

}

NodePtr Expr::get(Context &c, bool get)
{
    return primary(c, get);
}

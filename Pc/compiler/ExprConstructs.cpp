#include "ExprConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"

#include "compiler/CommonConstructs.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr expression(Context &c, bool get);

NodePtr addrOf(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::Amp, get);

    auto an = new AddrOfNode(tok.location());
    NodePtr n(an);

    an->expr = expression(c, true);

    return n;
}

NodePtr primary(Context &c, bool get)
{
    NodePtr n;

    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::Id: return CommonConstructs::extendedName(c, false);

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

        case Token::Type::Amp: return addrOf(c, false);

        default: throw Error(tok.location(), "expression expected - ", tok.text());
    }
}

void params(Context &c, NodeList &container, bool get)
{
    container.push_back(expression(c, get));

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        params(c, container, true);
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto cn = new CallNode(tok.location(), target);
    NodePtr n(cn);

    tok = c.scanner.next(true);
    if(tok.type() != Token::Type::RightParen)
    {
        params(c, cn->params, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    return n;
}

NodePtr entity(Context &c, bool get)
{
    auto n = primary(c, get);

    while(true)
    {
        switch(c.scanner.token().type())
        {
            case Token::Type::LeftParen: n = call(c, n, false); break;

            default: return n;
        }
    }
}

NodePtr assign(Context &c, bool get)
{
    auto n = entity(c, get);

    while(c.scanner.token().type() == Token::Type::Assign)
    {
        auto an = new AssignNode(c.scanner.token().location(), n);
        n = an;

        an->expr = expression(c, true);
    }

    return n;
}

NodePtr expression(Context &c, bool get)
{
    return assign(c, get);
}

}

NodePtr ExprConstructs::expr(Context &c, bool get)
{
    return expression(c, get);
}

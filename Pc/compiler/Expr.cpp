#include "Expr.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IntLiteralNode.h"
#include "nodes/CallNode.h"

#include "compiler/CommonConstructs.h"

#include "visitors/LookupVisitor.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr id(Context &c, bool get)
{
    auto nn = CommonConstructs::name(c, get);

    LookupVisitor lv(c);
    nn->accept(lv);

    return nn;
}

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    NodePtr n;

    switch(tok.type())
    {
        case Token::Type::Id:
        case Token::Type::Dot: return id(c, false);

        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

        default: break;
    }

    throw Error(tok.location(), "primary expected - ", tok.text());
}

void arg(Context &c, NodeList &args, bool get)
{
    auto a = Expr::get(c, get);
    args.push_back(a.release());

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        arg(c, args, true);
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto cn = new CallNode(tok.location(), target);

    c.scanner.next(true);
    if(c.scanner.token().type() != Token::Type::RightParen)
    {
        arg(c, cn->args, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    return cn;
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

NodePtr expression(Context &c, bool get)
{
    return entity(c, get);
}

}

NodePtr Expr::get(Context &c, bool get)
{
    return expression(c, get);
}

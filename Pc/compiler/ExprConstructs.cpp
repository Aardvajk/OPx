#include "ExprConstructs.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"

#include "compiler/CommonConstructs.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr expression(Context &c, bool get);
NodePtr entity(Context &c, bool get);

NodePtr parentheses(Context &c, bool get)
{
    c.scanner.match(Token::Type::LeftParen, get);

    auto n = expression(c, true);

    c.scanner.consume(Token::Type::RightParen, false);
    return n;
}

NodePtr addrOf(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::Amp, get);

    auto an = new AddrOfNode(tok.location());
    NodePtr n(an);

    an->expr = entity(c, true);

    return n;
}

NodePtr deref(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::Star, get);

    auto n = new DerefNode(tok.location());
    NodePtr nn = n;

    n->expr = entity(c, true);
    return nn;
}

NodePtr primary(Context &c, bool get)
{
    NodePtr n;

    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::Id: n = new IdNode(tok.location(), { }, tok.text()); c.scanner.next(true); return n;

        case Token::Type::RwThis: n = new ThisNode(tok.location()); c.scanner.next(true); return n;

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;
        case Token::Type::SizeLiteral: n = new SizeLiteralNode(tok.location(), pcx::lexical_cast<std::size_t>(tok.text())); c.scanner.next(true); return n;
        case Token::Type::StringLiteral: n = new StringLiteralNode(tok.location(), tok.text()); c.scanner.next(true); return n;

        case Token::Type::RwTrue: n = new BoolLiteralNode(tok.location(), true); c.scanner.next(true); return n;
        case Token::Type::RwFalse: n = new BoolLiteralNode(tok.location(), false); c.scanner.next(true); return n;

        case Token::Type::Amp: return addrOf(c, false);
        case Token::Type::Star: return deref(c, false);

        case Token::Type::LeftParen: return parentheses(c, false);

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

NodePtr dot(Context &c, NodePtr parent, bool get)
{
    auto tok = c.scanner.match(Token::Type::Dot, get);

    c.scanner.match(Token::Type::Id, true);

    auto n = new IdNode(tok.location(), parent, c.scanner.token().text());
    NodePtr nn(n);

    c.scanner.next(true);
    return nn;
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

NodePtr subscript(Context &c, NodePtr target, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftSub, get);

    auto sn = new SubscriptNode(tok.location(), target);
    NodePtr n(sn);

    sn->expr = expression(c, true);

    c.scanner.consume(Token::Type::RightSub, false);
    return n;
}

NodePtr entity(Context &c, bool get)
{
    auto n = primary(c, get);

    while(true)
    {
        switch(c.scanner.token().type())
        {
            case Token::Type::Dot: n = dot(c, n, false); break;
            case Token::Type::LeftParen: n = call(c, n, false); break;
            case Token::Type::LeftSub: n = subscript(c, n, false); break;

            default: return n;
        }
    }
}

NodePtr sums(Context &c, bool get)
{
    auto n = entity(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::Add: n = new BinaryNode(loc, Operators::Type::Add, n, entity(c, true)); break;

            default: return n;
        }
    }
}

NodePtr assign(Context &c, bool get)
{
    auto n = sums(c, get);

    while(c.scanner.token().type() == Token::Type::Assign)
    {
        auto an = new AssignNode(c.scanner.token().location(), n);
        n = an;

        an->expr = sums(c, true);
    }

    return n;
}

NodePtr comparisons(Context &c, bool get)
{
    auto n = assign(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::Eq: n = new BinaryNode(loc, Operators::Type::Eq, n, assign(c, true)); break;
            case Token::Type::Neq: n = new BinaryNode(loc, Operators::Type::Neq, n, assign(c, true)); break;

            default: return n;
        }
    }
}

NodePtr expression(Context &c, bool get)
{
    return comparisons(c, get);
}

}

NodePtr ExprConstructs::expr(Context &c, bool get)
{
    return expression(c, get);
}

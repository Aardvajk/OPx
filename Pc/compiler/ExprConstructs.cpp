#include "ExprConstructs.h"

#include "framework/Error.h"

#include "scanner/Lexer.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNode.h"
#include "nodes/OpEqNode.h"

#include "compiler/CommonConstructs.h"

#include "operators/Operators.h"

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

NodePtr unary(Context &c, Operators::Type op, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = new UnaryNode(tok.location(), op);
    NodePtr nn(n);

    n->expr = entity(c, true);
    return nn;
}

std::string convertId(Context &c, const Token &tok)
{
    if(tok.type() == Token::Type::RwOperator)
    {
        auto op = c.scanner.next(true);
        if(!Operators::allowed(op.type()))
        {
            throw Error(op.location(), "invalid operator - ", op.text());
        }

        return tok.text() + op.text();
    }
    else if(tok.type() != Token::Type::Id && tok.type() != Token::Type::RwNew && tok.type() != Token::Type::RwDelete)
    {
        throw Error(tok.location(), "identifier expected - ", tok.text());
    }

    return tok.text();
}

NodePtr id(Context &c, bool get)
{
    auto tok = c.scanner.next(get);
    auto value = convertId(c, tok);

    NodePtr n = new IdNode(tok.location(), { }, value);

    c.scanner.next(true);
    return n;
}

NodePtr preIncDec(Context &c, Operators::Type op, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = new IncDecNode(tok.location(), op);
    NodePtr nn(n);

    n->target = entity(c, true);

    return nn;
}

NodePtr primary(Context &c, bool get)
{
    NodePtr n;

    auto tok = c.scanner.next(get);
    switch(tok.type())
    {
        case Token::Type::Id:
        case Token::Type::RwOperator: return id(c, false);

        case Token::Type::RwThis: n = new ThisNode(tok.location()); c.scanner.next(true); return n;

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;
        case Token::Type::SizeLiteral: n = new SizeLiteralNode(tok.location(), pcx::lexical_cast<std::size_t>(tok.text())); c.scanner.next(true); return n;
        case Token::Type::StringLiteral: n = new StringLiteralNode(tok.location(), Lexer::decodeString(tok.text())); c.scanner.next(true); return n;

        case Token::Type::RwTrue: n = new BoolLiteralNode(tok.location(), true); c.scanner.next(true); return n;
        case Token::Type::RwFalse: n = new BoolLiteralNode(tok.location(), false); c.scanner.next(true); return n;

        case Token::Type::Amp: return addrOf(c, false);
        case Token::Type::Star: return deref(c, false);

        case Token::Type::LeftParen: return parentheses(c, false);

        case Token::Type::Exclaim: return unary(c, Operators::Type::Not, false);
        case Token::Type::Sub: return unary(c, Operators::Type::Neg, false);

        case Token::Type::Inc: return preIncDec(c, Operators::Type::PreInc, false);
        case Token::Type::Dec: return preIncDec(c, Operators::Type::PreDec, false);

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

    auto name = c.scanner.next(true);
    auto value = convertId(c, name);

    auto n = new IdNode(tok.location(), parent, value);
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

NodePtr postIncDec(Context &c, Operators::Type op, NodePtr target, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = new IncDecNode(tok.location(), op, target);
    NodePtr nn(n);

    c.scanner.next(true);

    return nn;
}

NodePtr opEq(Context &c, Operators::Type op, NodePtr target, bool get)
{
    auto tok = c.scanner.next(get);

    auto n = new OpEqNode(tok.location(), op, target);
    NodePtr nn(n);

    n->expr = expression(c, true);

    return nn;
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

            case Token::Type::Inc: n = postIncDec(c, Operators::Type::PostInc, n, false); break;
            case Token::Type::Dec: n = postIncDec(c, Operators::Type::PostDec, n, false); break;

            case Token::Type::AddEq: n = opEq(c, Operators::Type::AddEq, n, false); break;
            case Token::Type::SubEq: n = opEq(c, Operators::Type::SubEq, n, false); break;
            case Token::Type::MulEq: n = opEq(c, Operators::Type::MulEq, n, false); break;
            case Token::Type::DivEq: n = opEq(c, Operators::Type::DivEq, n, false); break;
            case Token::Type::ModEq: n = opEq(c, Operators::Type::ModEq, n, false); break;

            default: return n;
        }
    }
}

NodePtr terms(Context &c, bool get)
{
    auto n = entity(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::Star: n = new BinaryNode(loc, Operators::Type::Mul, n, entity(c, true)); break;
            case Token::Type::Div: n = new BinaryNode(loc, Operators::Type::Div, n, entity(c, true)); break;
            case Token::Type::Mod: n = new BinaryNode(loc, Operators::Type::Mod, n, entity(c, true)); break;

            default: return n;
        }
    }
}

NodePtr sums(Context &c, bool get)
{
    auto n = terms(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::Add: n = new BinaryNode(loc, Operators::Type::Add, n, terms(c, true)); break;
            case Token::Type::Sub: n = new BinaryNode(loc, Operators::Type::Sub, n, terms(c, true)); break;

            default: return n;
        }
    }
}

NodePtr shifts(Context &c, bool get)
{
    auto n = sums(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::LeftShift: n = new BinaryNode(loc, Operators::Type::LeftShift, n, sums(c, true)); break;

            default: return n;
        }
    }
}

NodePtr assign(Context &c, bool get)
{
    auto n = shifts(c, get);

    while(c.scanner.token().type() == Token::Type::Assign)
    {
        auto an = new AssignNode(c.scanner.token().location(), n);
        n = an;

        an->expr = expression(c, true);
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

            case Token::Type::Lt: n = new BinaryNode(loc, Operators::Type::Lt, n, assign(c, true)); break;
            case Token::Type::LtEq: n = new BinaryNode(loc, Operators::Type::LtEq, n, assign(c, true)); break;
            case Token::Type::Gt: n = new BinaryNode(loc, Operators::Type::Gt, n, assign(c, true)); break;
            case Token::Type::GtEq: n = new BinaryNode(loc, Operators::Type::GtEq, n, assign(c, true)); break;

            default: return n;
        }
    }
}

NodePtr logical(Context &c, bool get)
{
    auto n = comparisons(c, get);

    while(true)
    {
        auto loc = c.scanner.token().location();
        switch(c.scanner.token().type())
        {
            case Token::Type::And: n = new LogicalNode(loc, Operators::Type::And, n, comparisons(c, true)); break;
            case Token::Type::Or: n = new LogicalNode(loc, Operators::Type::Or, n, comparisons(c, true)); break;

            default: return n;
        }
    }
}

NodePtr expression(Context &c, bool get)
{
    return logical(c, get);
}

}

NodePtr ExprConstructs::expr(Context &c, bool get)
{
    return expression(c, get);
}

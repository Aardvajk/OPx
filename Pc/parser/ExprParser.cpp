#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "operators/Operators.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNodes.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr entity(Context &c, bool get);
NodePtr expression(Context &c, bool get);

std::string convertId(Context &c, const Token &tok)
{
    if(tok.type() == Token::Type::RwOperator)
    {
        auto s = Operators::scan(c, true);
        return pcx::str(tok.text(), s);
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

NodePtr parentheses(Context &c, bool get)
{
    auto tok = c.scanner.match(Token::Type::LeftParen, get);

    auto n = expression(c, true);

    c.scanner.consume(Token::Type::RightParen, false);
    return n;
}

template<typename N> NodePtr exprNode(Context &c, bool get)
{
    auto n = new N(c.scanner.token().location());
    NodePtr nn(n);

    n->expr = entity(c, get);

    return nn;
}

template<typename T> NodePtr tokenExprNode(Context &c, const Token &token, bool get)
{
    auto n = new T(c.scanner.token().location(), token);
    NodePtr nn(n);

    n->expr = entity(c, get);

    return nn;
}

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    NodePtr n;
    switch(tok.type())
    {
        case Token::Type::Id:
        case Token::Type::RwOperator: return id(c, false);

        case Token::Type::RwThis: n = new ThisNode(tok.location()); c.scanner.next(true); return n;

        case Token::Type::CharLiteral: n = new CharLiteralNode(tok.location(), tok.text()[0]); c.scanner.next(true); return n;
        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;
        case Token::Type::StringLiteral: n = new StringLiteralNode(tok.location(), Lexer::decodeString(tok.text())); c.scanner.next(true); return n;

        case Token::Type::RwTrue: n = new BoolLiteralNode(tok.location(), true); c.scanner.next(true); return n;
        case Token::Type::RwFalse: n = new BoolLiteralNode(tok.location(), false); c.scanner.next(true); return n;

        case Token::Type::LeftParen: return parentheses(c, false);

        case Token::Type::Amp: return exprNode<AddrOfNode>(c, true);
        case Token::Type::Star: return exprNode<DerefNode>(c, true);

        case Token::Type::Sub:
        case Token::Type::Exclaim: return tokenExprNode<UnaryNode>(c, tok, true);

        case Token::Type::Inc:
        case Token::Type::Dec: return tokenExprNode<PreIncDecNode>(c, tok, true);

        default: throw Error(tok.location(), "primary expected - ", tok.text());
    }
}

NodePtr dot(Context &c, NodePtr parent, bool get)
{
    auto name = c.scanner.next(get);
    auto value = convertId(c, name);

    auto n = new IdNode(name.location(), parent, value);
    NodePtr nn(n);

    c.scanner.next(true);
    return nn;
}

void params(Context &c, NodeList &container, bool get)
{
    auto n = expression(c, get);
    container.push_back(n);

    if(c.scanner.token().type() == Token::Type::Comma)
    {
        params(c, container, true);
    }
}

NodePtr call(Context &c, NodePtr target, bool get)
{
    auto n = new CallNode(c.scanner.token().location(), target);
    NodePtr nn(n);

    auto tok = c.scanner.next(true);
    if(tok.type() != Token::Type::RightParen)
    {
        params(c, n->params, false);
    }

    c.scanner.consume(Token::Type::RightParen, false);
    return nn;
}

NodePtr entity(Context &c, bool get)
{
    auto n = primary(c, get);

    while(true)
    {
        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::Dot: n = dot(c, n, true); break;
            case Token::Type::LeftParen: n = call(c, n, true); break;

            case Token::Type::Inc:
            case Token::Type::Dec: n = new PostIncDecNode(tok.location(), tok, n); c.scanner.next(true); break;

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

        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::Star:
            case Token::Type::Div:
            case Token::Type::Mod: n = new BinaryNode(loc, tok, n, entity(c, true)); break;

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

        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::Add:
            case Token::Type::Sub: n = new BinaryNode(loc, tok, n, terms(c, true)); break;

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

        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::LeftShift: n = new BinaryNode(loc, tok, n, sums(c, true)); break;

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

        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::Eq:
            case Token::Type::Neq:
            case Token::Type::Lt:
            case Token::Type::LtEq:
            case Token::Type::Gt:
            case Token::Type::GtEq: n = new BinaryNode(loc, tok, n, assign(c, true)); break;

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

        auto tok = c.scanner.token();
        switch(tok.type())
        {
            case Token::Type::And:
            case Token::Type::Or: n = new LogicalNode(loc, tok, n, comparisons(c, true)); break;

            default: return n;
        }
    }
}

NodePtr expression(Context &c, bool get)
{
    return logical(c, get);
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return expression(c, get);
}

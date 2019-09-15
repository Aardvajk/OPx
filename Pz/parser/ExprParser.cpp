#include "ExprParser.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "operators/Operators.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"

#include <pcx/lexical_cast.h>

namespace
{

NodePtr expression(Context &c, bool get);

std::string convertId(Context &c, const Token &tok)
{
    if(tok.type() == Token::Type::RwOperator)
    {
        auto op = Operators::scan(c, true);
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

NodePtr primary(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    NodePtr n;
    switch(tok.type())
    {
        case Token::Type::Id:
        case Token::Type::RwOperator: return id(c, false);

        case Token::Type::IntLiteral: n = new IntLiteralNode(tok.location(), pcx::lexical_cast<int>(tok.text())); c.scanner.next(true); return n;

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
        switch(c.scanner.token().type())
        {
            case Token::Type::Dot: n = dot(c, n, true); break;
            case Token::Type::LeftParen: n = call(c, n, true); break;

            default: return n;
        }
    }
}

NodePtr expression(Context &c, bool get)
{
    return entity(c, get);
}

}

NodePtr ExprParser::build(Context &c, bool get)
{
    return expression(c, get);
}

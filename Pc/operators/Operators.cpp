#include "Operators.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "parser/TypeParser.h"

#include "nodes/TextNode.h"

NodePtr Operators::scan(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::LeftParen)
    {
        c.scanner.consume(Token::Type::RightParen, true);
        return new TextNode(tok.location(), "()");
    }

    if(tok.type() == Token::Type::LeftSub)
    {
        c.scanner.consume(Token::Type::RightSub, true);
        return new TextNode(tok.location(), "[]");
    }

    switch(tok.type())
    {
        case Token::Type::Assign:

        case Token::Type::Add:
        case Token::Type::Sub:
        case Token::Type::Star:
        case Token::Type::Div:
        case Token::Type::Mod:

        case Token::Type::AddEq:
        case Token::Type::SubEq:
        case Token::Type::MulEq:
        case Token::Type::DivEq:
        case Token::Type::ModEq:

        case Token::Type::Exclaim:

        case Token::Type::Eq:
        case Token::Type::Neq:
        case Token::Type::Lt:
        case Token::Type::LtEq:
        case Token::Type::Gt:
        case Token::Type::GtEq:

        case Token::Type::Inc:
        case Token::Type::Dec:

        case Token::Type::LeftShift: c.scanner.next(true); return new TextNode(tok.location(), tok.text());

        default: return TypeParser::build(c, false);
    }
}

Token Operators::opEqToOp(const Token &token)
{
    switch(token.type())
    {
        case Token::Type::AddEq: return Token(Token::Type::Add, token.location(), "+");
        case Token::Type::SubEq: return Token(Token::Type::Sub, token.location(), "-");
        case Token::Type::MulEq: return Token(Token::Type::Star, token.location(), "*");
        case Token::Type::DivEq: return Token(Token::Type::Div, token.location(), "/");
        case Token::Type::ModEq: return Token(Token::Type::Mod, token.location(), "%");

        default: return { };
    }
}

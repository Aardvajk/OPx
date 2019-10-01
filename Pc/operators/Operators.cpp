#include "Operators.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "parser/CommonParser.h"

#include "nodes/TextNode.h"

NodePtr Operators::scan(Context &c, bool get)
{
    auto tok = c.scanner.next(get);

    if(tok.type() == Token::Type::LeftParen)
    {
        c.scanner.consume(Token::Type::RightParen, true);
        return new TextNode(tok.location(), "()");
    }

    switch(tok.type())
    {
        case Token::Type::Assign:

        case Token::Type::Add:
        case Token::Type::Sub:
        case Token::Type::Star:
        case Token::Type::Div:
        case Token::Type::Mod:

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

        default: return CommonParser::name(c, false);
    }
}

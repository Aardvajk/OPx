#include "Operators.h"

#include "framework/Error.h"

#include "scanner/Scanner.h"

namespace
{

bool allowed(Token::Type v)
{
    switch(v)
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

        case Token::Type::AddEq:
        case Token::Type::SubEq:
        case Token::Type::MulEq:
        case Token::Type::DivEq:
        case Token::Type::ModEq:

        case Token::Type::LeftShift:

        case Token::Type::CallOp: return true;

        default: return false;
    };
}

}

const char *Operators::toString(Type v)
{
    static const char *s[] =
    {
        "=",

        "+",
        "-",
        "*",
        "/",
        "%",

        "!",
        "-",

        "==",
        "!=",

        "<",
        "<=",
        ">",
        ">=",

        "&&",
        "||",

        "++",
        "--",
        "++",
        "--",

        "+=",
        "-=",
        "*=",
        "/=",
        "%=",

        "<<",

        "()"
    };

    return s[static_cast<int>(v)];
}

Token Operators::scan(Scanner &scanner, bool get)
{
    auto tok = scanner.next(true);
    if(tok.type() == Token::Type::LeftParen)
    {
        scanner.match(Token::Type::RightParen, true);
        return Token(Token::Type::CallOp, tok.location(), "()");
    }

    if(allowed(tok.type()))
    {
        return tok;
    }

    throw Error(tok.location(), "invalid operator - ", tok.text());
}

Operators::Type Operators::opFromOpEq(Type v)
{
    switch(v)
    {
        case Type::AddEq: return Type::Add;
        case Type::SubEq: return Type::Sub;
        case Type::MulEq: return Type::Mul;
        case Type::DivEq: return Type::Div;
        case Type::ModEq: return Type::Mod;

        default: return { };
    }
}


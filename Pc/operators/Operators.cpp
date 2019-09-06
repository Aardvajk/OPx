#include "Operators.h"

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

        "<<"
    };

    return s[static_cast<int>(v)];
}

bool Operators::allowed(Token::Type v)
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

        case Token::Type::LeftShift: return true;

        default: return false;
    };
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


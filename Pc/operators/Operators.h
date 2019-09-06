#ifndef OPERATORS_H
#define OPERATORS_H

#include "scanner/Token.h"

namespace Operators
{

enum class Type
{
    Assign,

    Add,
    Sub,
    Mul,
    Div,
    Mod,

    Not,
    Neg,

    Eq,
    Neq,

    Lt,
    LtEq,
    Gt,
    GtEq,

    And,
    Or,

    PreInc,
    PreDec,
    PostInc,
    PostDec,

    AddEq,
    SubEq,
    MulEq,
    DivEq,
    ModEq,

    LeftShift
};

const char *toString(Type v);
bool allowed(Token::Type v);

Type opFromOpEq(Type v);

}

#endif // OPERATORS_H

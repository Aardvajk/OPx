#ifndef OPERATORS_H
#define OPERATORS_H

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
    Or
};

const char *toString(Type v);

}

#endif // OPERATORS_H

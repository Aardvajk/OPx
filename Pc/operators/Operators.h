#ifndef OPERATORS_H
#define OPERATORS_H

namespace Operators
{

enum class Type
{
    Assign,
    Add,
    Eq,
    Neq
};

const char *toString(Type v);

}

#endif // OPERATORS_H

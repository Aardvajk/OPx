#ifndef OPERATORS_H
#define OPERATORS_H

namespace Operators
{

enum class Type
{
    Assign,
    Add
};

const char *toString(Type v);

}

#endif // OPERATORS_H

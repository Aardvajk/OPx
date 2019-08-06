#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <cstdint>

namespace Primitive
{

enum class Type : std::uint8_t
{
    Null,
    Char,
    Int,
    Size,

    Invalid
};

const char *toString(Type v);

}

#endif // PRIMITIVE_H

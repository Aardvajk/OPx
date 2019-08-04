#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <cstdint>

namespace Primitive
{

enum class Type : std::uint8_t
{
    Char,
    Int,
    Size
};

const char *toString(Type v);

}

#endif // PRIMITIVE_H

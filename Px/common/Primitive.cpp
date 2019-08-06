#include "Primitive.h"

const char *Primitive::toString(Type v)
{
    static const char *s[] =
    {
        "null",
        "char",
        "int",
        "size",

        "invalid"
    };

    return s[static_cast<int>(v)];
}

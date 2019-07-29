#include "Operators.h"

const char *Operators::toString(Type v)
{
    static const char *s[] =
    {
        "=",
        "+"
    };

    return s[static_cast<int>(v)];
}

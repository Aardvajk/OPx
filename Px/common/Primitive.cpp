#include "Primitive.h"

const char *Primitive::toString(Type v)
{
    static const char *s[] = { "char", "int", "size" };
    return s[static_cast<int>(v)];
}

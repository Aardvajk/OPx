#include "Type.h"

Type Type::makePrimary(Sym *sym)
{
    Type t;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(Type *returnType)
{
    Type t;
    t.returnType = returnType;

    return t;
}

Type::Type() : sym(nullptr), returnType(nullptr)
{
}

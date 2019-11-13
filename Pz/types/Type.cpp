#include "Type.h"

Type::Type() : constant(false), ref(false), ptr(0), sym(nullptr), returnType(nullptr)
{
}

Type Type::makePrimary(Sym *sym)
{
    Type t;
    t.sym = sym;

    return t;
}

Type Type::makePrimary(bool constant, bool ref, Sym *sym)
{
    Type t;

    t.constant = constant;
    t.ref = ref;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(Type *returnType, const std::vector<Type*> &args)
{
    Type t;
    t.args = args;
    t.returnType = returnType;

    return t;
}


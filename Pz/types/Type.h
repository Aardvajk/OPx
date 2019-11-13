#ifndef TYPE_H
#define TYPE_H

#include <vector>

class Sym;

class Type
{
public:
    Type();

    bool constant;
    bool ref;

    std::size_t ptr;

    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    static Type makePrimary(Sym *sym);
    static Type makePrimary(bool constant, bool ref, Sym *sym);
    static Type makeFunction(Type *returnType, const std::vector<Type*> &args = { });
};

#endif // TYPE_H

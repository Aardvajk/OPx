#ifndef TYPE_H
#define TYPE_H

#include <vector>

class Sym;

class Type
{
public:
    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    static Type makePrimary(Sym *sym);
    static Type makeFunction(Type *returnType);

private:
    Type();
};

#endif // TYPE_H

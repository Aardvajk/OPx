#ifndef TYPE_H
#define TYPE_H

#include "types/GenericRef.h"

#include <pcx/optional.h>

#include <vector>

class Sym;

class Type
{
public:
    Type();

    std::string text() const;

    bool constant;
    bool ref;

    std::size_t ptr;

    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    pcx::optional<GenericRef> genericRef;

    static Type makePrimary(Sym *sym);
    static Type makePrimary(bool constant, bool ref, Sym *sym);
    static Type makeFunction(Type *returnType, const std::vector<Type*> &args = { });
    static Type makeGeneric(const GenericRef &ref);
};

#endif // TYPE_H

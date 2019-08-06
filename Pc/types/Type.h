#ifndef TYPE_H
#define TYPE_H

#include "common/Primitive.h"

#include <pcx/optional.h>

#include <vector>
#include <string>

class Sym;

class Type
{
public:
    pcx::optional<std::size_t> size() const;
    pcx::optional<std::size_t> initSize() const;

    std::string text() const;
    bool function() const;
    bool primitive() const;
    Primitive::Type primitiveType() const;

    static Type makePrimary(unsigned ptr, Sym *sym);
    static Type makeFunction(unsigned ptr, Type *returnType);
    static Type removeSub(const Type &type);

    unsigned ptr;
    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    std::size_t sub;

    bool method;

private:
    Type();
};

#endif // TYPE_H

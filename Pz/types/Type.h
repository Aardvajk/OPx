#ifndef TYPE_H
#define TYPE_H

#include "scanner/Location.h"

#include <string>
#include <vector>

#include <pcx/optional.h>

class Sym;

class Type
{
public:
    std::string text() const;

    pcx::optional<std::size_t> size() const;

    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    static Type makePrimary(Sym *sym);
    static Type makeFunction(Type *returnType);

    static std::size_t assertSize(Location location, const Type *type);

private:
    Type();
};

#endif // TYPE_H

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
    Type();

    Type addPointer() const;
    Type removePointer() const;

    std::string text() const;

    bool function() const;
    bool primitive() const;

    pcx::optional<std::size_t> size() const;

    bool constant;
    bool ref;
    std::size_t ptr;

    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    bool method;
    bool constMethod;

    static Type makePrimary(Sym *sym);
    static Type makeFunction(Type *returnType);

    static std::size_t assertSize(Location location, const Type *type);
};

#endif // TYPE_H

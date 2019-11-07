#ifndef TYPE_H
#define TYPE_H

#include "common/Primitive.h"

#include "scanner/Location.h"

#include "generics/GenericRef.h"

#include <string>
#include <vector>

#include <pcx/optional.h>
#include <pcx/flags.h>

class Sym;

class Type
{
public:
    Type();

    Type addPointer() const;
    Type removePointer() const;
    Type addReference() const;
    Type removeReference() const;

    std::string text() const;

    bool function() const;
    bool primitive() const;
    bool primitiveOrRef() const;

    Primitive::Type primitiveType() const;

    pcx::optional<std::size_t> size() const;

    std::size_t genericTypeCount() const;

    static Type makePrimary(Sym *sym);
    static Type makePrimary(bool constant, bool ref, Sym *sym);
    static Type makeFunction(Type *returnType, const std::vector<Type*> &args = { });
    static Type makeGeneric(const GenericRef &ref);

    static std::size_t assertSize(Location location, const Type *type);

    static std::vector<Type*> nonDefaultArgs(Sym *sym);

    bool constant;
    bool ref;
    std::size_t ptr;

    Sym *sym;

    std::vector<Type*> args;
    Type *returnType;

    bool method;
    bool constMethod;

    pcx::optional<GenericRef> generic;
};

#endif // TYPE_H

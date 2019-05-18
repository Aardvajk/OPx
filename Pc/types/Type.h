#ifndef TYPE_H
#define TYPE_H

#include <pcx/scoped_ptr.h>
#include <pcx/ptr_vector.h>
#include <pcx/optional.h>

#include <string>

class Sym;

class Type
{
public:
    explicit Type(Sym *sym = nullptr);

    Type *clone() const;

    std::string text() const;
    pcx::optional<std::size_t> size() const;

    Sym *sym;
    unsigned ptr;

    pcx::scoped_ptr<Type> returnType;
    pcx::ptr_vector<Type> args;
};

using TypePtr = pcx::scoped_ptr<Type>;

#endif // TYPE_H

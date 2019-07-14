#ifndef TYPE_H
#define TYPE_H

#include <pcx/optional.h>

#include <vector>
#include <string>

class Sym;

class Type
{
public:
    Type();
    Type(unsigned ptr, Sym *sym);

    pcx::optional<std::size_t> size() const;

    std::string text() const;

    unsigned ptr;
    Sym *sym;

    std::vector<const Type*> args;
    const Type *returnType;
};

#endif // TYPE_H

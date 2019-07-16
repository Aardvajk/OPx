#ifndef TYPE_H
#define TYPE_H

#include <pcx/optional.h>

#include <vector>
#include <string>

class Sym;

class Type
{
public:
    pcx::optional<std::size_t> size() const;

    std::string text() const;
    bool function() const { return returnType; }

    static Type makePrimary(unsigned ptr, const Sym *sym);
    static Type makeFunction(unsigned ptr, const Type *returnType);

    unsigned ptr;
    const Sym *sym;

    std::vector<const Type*> args;
    const Type *returnType;

private:
    Type();
};

#endif // TYPE_H

#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include <string>

class Sym;

class Type
{
public:
    Type();
    Type(unsigned ptr, Sym *sym);
    ~Type();

    std::string text() const;

    unsigned ptr;
    Sym *sym;

    std::vector<const Type*> args;
    const Type *returnType;
};

#endif // TYPE_H

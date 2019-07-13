#ifndef TYPE_H
#define TYPE_H

#include <pcx/scoped_ptr.h>
#include <pcx/ptr_vector.h>

#include <string>

class Sym;

class Type
{
public:
    explicit Type(Sym *sym = nullptr);

    std::string text() const;

    unsigned ptr;
    Sym *sym;

    pcx::ptr_vector<Type> args;
    pcx::scoped_ptr<Type> returnType;
};

#endif // TYPE_H

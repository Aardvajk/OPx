#ifndef GENERIC_H
#define GENERIC_H

#include <string>
#include <vector>

class Context;
class Type;
class Sym;

class Generic
{
public:
    explicit Generic(std::string name, Type *type = nullptr);

    static std::string funcName(Context &c, Sym *sym, const std::vector<Type*> &types);

    template<typename T, typename S> static void combineTypes(T &type, const S &source);

    std::string name;
    Type *type;
};

template<typename T, typename S> void Generic::combineTypes(T &type, const S &source)
{
    if(!type.constant)
    {
        type.constant = source.constant;
    }

    if(!type.ref)
    {
        type.ref = source.ref;
    }

    if(!type.ptr)
    {
        type.ptr = source.ptr;
    }
}

#endif // GENERIC_H

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

    std::string name;
    Type *type;
};

#endif // GENERIC_H

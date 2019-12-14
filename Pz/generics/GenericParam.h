#ifndef GENERICPARAM_H
#define GENERICPARAM_H

#include <string>

class Type;

class GenericParam
{
public:
    explicit GenericParam(std::string name) : name(std::move(name)), type(nullptr) { }

    std::string name;
    Type *type;
};

#endif // GENERICPARAM_H

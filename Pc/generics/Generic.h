#ifndef GENERIC_H
#define GENERIC_H

#include <string>

class Type;

class Generic
{
public:
    explicit Generic(std::string name, Type *type = nullptr);

    std::string name;
    Type *type;
};

#endif // GENERIC_H

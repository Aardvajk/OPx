#ifndef TEMP_H
#define TEMP_H

#include <string>

class Type;

class Temp
{
public:
    Temp(std::string name, Type *type, bool zeroFlag = false) : name(std::move(name)), type(type), zeroFlag(zeroFlag) { }

    std::string name;
    Type *type;
    bool zeroFlag;
};

#endif // TEMP_H

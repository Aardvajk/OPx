#ifndef TEMPDESTRUCT_H
#define TEMPDESTRUCT_H

#include <string>

class Type;

class TempDestruct
{
public:
    TempDestruct(std::string name, Type *type, std::string flag = { }) : name(std::move(name)), type(type), flag(std::move(flag)) { }

    std::string name;
    Type *type;
    std::string flag;
};

#endif // TEMPDESTRUCT_H

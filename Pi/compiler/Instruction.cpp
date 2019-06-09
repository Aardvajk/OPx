#include "Instruction.h"

namespace
{

const char *s[] =
{
    "jmp",

    "push",
    "pop",

    "addr",

    "svc",
    ""
};

}

const char *Instruction::toString(Type value)
{
    return s[static_cast<int>(value)];
}

Instruction::Type Instruction::fromString(const std::string &value)
{
    for(int i = 0; s[i][0]; ++i)
    {
        if(s[i] == value)
        {
            return static_cast<Type>(i);
        }
    }

    return Type::Invalid;
}

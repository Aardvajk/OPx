#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

namespace Instruction
{

enum class Type
{
    Jmp,
    Call,

    Push,
    Pop,
    AllocS,

    Store,

    Svc,
    Invalid
};

const char *toString(Type value);
Type fromString(const std::string &value);

}

#endif // INSTRUCTION_H

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

namespace Instruction
{

enum class Code
{
    Push,
    Pop,
    Store,

    Jmp,
    Call,

    Int,
    Foo,
    Invalid
};

const char *toString(Code code);
Code fromString(const std::string &text);

}

#endif // INSTRUCTION_H

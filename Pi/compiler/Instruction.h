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

    Load,
    Store,

    Add,
    Sub,
    Mul,
    Div,
    Mod,

    Not,
    Neg,

    Lt,
    LtEq,

    Convert,

    Alloc,
    Free,

    Svc,
    Invalid
};

const char *toString(Type value);
Type fromString(const std::string &value);

}

#endif // INSTRUCTION_H

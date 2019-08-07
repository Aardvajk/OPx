#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>

namespace OpCode
{

enum class Reg : std::uint8_t
{
    Pc,
    Bp,
    Sp,
    Dx,
    Invalid
};

const char *toString(Reg v);

enum class Op : std::uint8_t
{
    End,

    SetRI,

    SubRI,
    AddRI,

    CopyRR,
    CopyRA,
    CopyAR,
    CopyAI,
    CopyAA,

    PushR,
    PopR,

    Call,
    Ret,
    JmpZ,

    Add,
    Sub,
    Mul,
    Div,
    Mod,

    Not,
    Neg,

    Lt,
    LtEq,

    Conv,

    Alloc,
    Free,

    Service,

    Invalid
};

const char *toString(Op v);

}

#endif // OPCODE_H

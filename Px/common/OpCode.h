#ifndef OPCODE_H
#define OPCODE_H


namespace OpCode
{

enum class Reg : unsigned char
{
    Pc,
    Bp,
    Sp,
    Dx,
    Invalid
};

const char *toString(Reg v);

enum class Op : unsigned char
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

    Int,

    Invalid
};

const char *toString(Op v);

}

#endif // OPCODE_H

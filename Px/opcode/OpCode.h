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

enum class Type : unsigned char
{
    End,

    SubRI,
    AddRI,

    CopyRR,
    CopyRA,
    CopyAR,
    CopyAA,
    CopyAI,

    PushR,
    PopR,

    Int,

    Invalid
};

const char *toString(Type v);

}

#endif // OPCODE_H

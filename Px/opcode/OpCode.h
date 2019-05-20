#ifndef OPCODE_H
#define OPCODE_H


namespace OpCode
{

enum class Reg : unsigned char
{
    Pc,
    Bp,
    Sp,
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

    PrintR,

    Invalid
};

const char *toString(Type v);

}

#endif // OPCODE_H

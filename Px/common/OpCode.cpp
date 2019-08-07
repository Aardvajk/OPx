#include "OpCode.h"

const char *OpCode::toString(Reg v)
{
    static const char *s[] =
    {
        "pc",
        "bp",
        "sp",
        "dx",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

const char *OpCode::toString(Op v)
{
    static const char *s[] =
    {
        "end",

        "setri",

        "subri",
        "addri",

        "copyrr",
        "copyra",
        "copyar",
        "copyai",
        "copyaa",

        "pushr",
        "popr",

        "call",
        "ret",
        "jmpz",

        "add",
        "sub",
        "mul",
        "div",
        "mod",

        "not",
        "neg",

        "lt",
        "lteq",

        "conv",

        "alloc",
        "free",

        "service",

        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

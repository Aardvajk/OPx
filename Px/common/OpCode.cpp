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

        "subri",
        "addri",

        "copyrr",
        "copyra",
        "copyar",
        "copyaa",

        "pushr",
        "popr",

        "int",

        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

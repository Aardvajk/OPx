#include "OpCode.h"

const char *OpCode::toString(Reg v)
{
    static const char *s[] =
    {
        "pc",
        "bp",
        "sp",
        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

const char *OpCode::toString(Type v)
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

        "printr",

        "(invalid)"
    };

    return s[static_cast<int>(v)];
}

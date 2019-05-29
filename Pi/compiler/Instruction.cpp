#include "Instruction.h"

namespace
{

const char *s[] =
{
    "push",
    "pop",
    "store",

    "jmp",
    "call",

    "int",
    "foo",
    ""
};

}

const char *Instruction::toString(Code code)
{
    return s[static_cast<int>(code)];
}

Instruction::Code Instruction::fromString(const std::string &text)
{
    for(int i = 0; s[i][0]; ++i)
    {
        if(s[i] == text) return static_cast<Code>(i);
    }

    return Code::Invalid;
}

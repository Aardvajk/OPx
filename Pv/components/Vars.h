#ifndef VARS_H
#define VARS_H

#include "opcode/OpCode.h"

#include <cstring>

struct Vars
{
    OpCode::Type op;
    OpCode::Reg r0, r1;
    std::size_t s0;
    int i0;
};

#endif // VARS_H

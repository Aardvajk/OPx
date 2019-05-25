#ifndef VARS_H
#define VARS_H

#include "common/OpCode.h"

class Vars
{
public:
    OpCode::Op op;
    OpCode::Reg r0, r1;
    std::size_t s0;
    int i0;
};

#endif // VARS_H

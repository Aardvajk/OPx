#ifndef REGISTERS_H
#define REGISTERS_H

#include "common/OpCode.h"

#include <cstring>

class Registers
{
public:
    Registers();

    std::size_t &operator[](OpCode::Reg i){ return v[static_cast<int>(i)]; }

    std::size_t &pc(){ return operator[](OpCode::Reg::Pc); }
    std::size_t &bp(){ return operator[](OpCode::Reg::Bp); }
    std::size_t &sp(){ return operator[](OpCode::Reg::Sp); }
    std::size_t &dx(){ return operator[](OpCode::Reg::Dx); }

private:
    std::size_t v[5];
};

#endif // REGISTERS_H

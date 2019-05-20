#ifndef REGISTERS_H
#define REGISTERS_H

#include "opcode/OpCode.h"

#include <cstring>

class Registers
{
public:
    Registers();

    char *&operator[](OpCode::Reg i){ return reg(i); }

    char *&pc(){ return v[static_cast<int>(OpCode::Reg::Pc)]; }
    char *&bp(){ return v[static_cast<int>(OpCode::Reg::Bp)]; }
    char *&sp(){ return v[static_cast<int>(OpCode::Reg::Sp)]; }

    void ra(OpCode::Reg s, OpCode::Reg d){ std::memcpy(reg(d), &(reg(s)), sizeof(char*)); }
    void ar(OpCode::Reg s, OpCode::Reg d){ std::memcpy(&(reg(d)), reg(s), sizeof(char*)); }
    void aa(OpCode::Reg s, OpCode::Reg d, std::size_t n){ std::memcpy(reg(d), reg(s), n); }

private:
    char *&reg(OpCode::Reg i){ return v[static_cast<int>(i)]; }

    char *v[4];
};

#endif // REGISTERS_H

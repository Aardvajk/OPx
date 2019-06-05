#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <cstring>
#include <iostream>

class Context;

class Disassembler
{
public:
    virtual ~Disassembler();

    virtual void map(Context &c, std::size_t pc);

    void disassemble(Context &c, std::ostream &os, const char *data, std::size_t size);
};

#endif // DISASSEMBLER_H

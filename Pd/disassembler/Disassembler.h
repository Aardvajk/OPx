#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <cstring>
#include <iostream>

class Context;

class Disassembler
{
public:
    Disassembler(std::size_t index, std::size_t offset = 0);

    void map(Context &c, std::size_t &pc);
    void disassemble(Context &c, std::ostream &os, const char *data, std::size_t size);

private:
    std::size_t index;
    std::size_t offset;
    std::size_t i;
};

#endif // DISASSEMBLER_H

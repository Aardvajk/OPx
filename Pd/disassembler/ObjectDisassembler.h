#ifndef OBJECTDISASSEMBLER_H
#define OBJECTDISASSEMBLER_H

#include "disassembler/Disassembler.h"

class ObjectDisassembler : public Disassembler
{
public:
    explicit ObjectDisassembler(std::size_t index);

    virtual void map(Context &c, std::size_t pc) override;

private:
    std::size_t index;
    std::size_t i;
};

#endif // OBJECTDISASSEMBLER_H

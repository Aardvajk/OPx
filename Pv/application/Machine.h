#ifndef MACHINE_H
#define MACHINE_H

#include "components/Memory.h"
#include "components/Registers.h"

#include <vector>

class Machine
{
public:
    using IntFunc = void(*)(int,Memory&,Registers&);

    Machine(const std::vector<char> &pr, IntFunc intFunc);

    void execute();

private:
    Memory mm;
    Registers rg;
    IntFunc ip;
};

#endif // MACHINE_H

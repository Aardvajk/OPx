#ifndef MACHINE_H
#define MACHINE_H

#include "components/Memory.h"
#include "components/Registers.h"

#include <vector>

class Machine
{
public:
    using IntProc = void(*)(int, Memory&, Registers&);

    Machine(const std::vector<char> &v, IntProc ip);

    void execute();
    void shutdown();

private:
    Memory mm;
    Registers rg;
    IntProc ip;
};

#endif // MACHINE_H

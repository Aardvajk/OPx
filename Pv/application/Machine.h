#ifndef MACHINE_H
#define MACHINE_H

#include "components/Memory.h"
#include "components/Registers.h"

#include <vector>

class Machine
{
public:
    Machine(const std::vector<char> &v);

    void execute();
    void shutdown();

private:
    Memory mm;
    Registers rg;
};

#endif // MACHINE_H

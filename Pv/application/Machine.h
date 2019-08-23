#ifndef MACHINE_H
#define MACHINE_H

#include "components/Memory.h"
#include "components/Registers.h"
#include "components/FreeStore.h"

#include <vector>

class Machine
{
public:
    using ServiceProc = void(*)(int, Memory&, Registers&);

    Machine(const std::vector<char> &v, ServiceProc sp);

    void execute();
    void shutdown(bool quiet);

private:
    Memory mm;
    Registers rg;
    FreeStore fs;
    ServiceProc sp;
};

#endif // MACHINE_H

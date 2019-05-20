#ifndef MACHINE_H
#define MACHINE_H

#include "components/Registers.h"

#include <vector>

class Machine
{
public:
    Machine(const std::vector<char> &pr);

    void execute();

private:
    std::vector<char> mm;
    Registers rg;
};

#endif // MACHINE_H

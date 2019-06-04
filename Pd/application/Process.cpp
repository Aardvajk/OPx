#include "Process.h"

#include "framework/Console.h"

#include "application/Context.h"
#include "application/Disassmble.h"

#include <pcx/join_str.h>
#include <pcx/str.h>

namespace
{

void processValue(Context &c, std::size_t index)
{
    std::cout << pcx::join_str(c.segments[index], ",", [](char c){ return pcx::str(int(c)); }) << "\n";
}

void processFunction(Context &c, std::size_t index)
{
    Disassemble::disassemble(c, std::cout, c.segments[index].data(), c.segments[index].size());
}

}

void Process::process(Context &c)
{
    auto &u = c.unit;

    for(std::size_t i = 0; i < c.unit.entities.size(); ++i)
    {
        auto &e = u.entities[i];

        std::cout << banner("disassemble ", u.strings[e.id]);

        switch(e.type)
        {
            case 'V': processValue(c, i); break;
            case 'F': processFunction(c, i); break;
        }
    }
}

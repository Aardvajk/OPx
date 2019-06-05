#include "Process.h"

#include "framework/Console.h"
#include "framework/LoadBinaryFile.h"

#include "application/Context.h"
#include "application/Loader.h"

#include "disassembler/ObjectDisassembler.h"

#include <pcx/join_str.h>
#include <pcx/str.h>
#include <pcx/indexed_range.h>

namespace
{

void processValue(Context &c, std::size_t index)
{
    std::size_t max = 8;
    std::vector<char> v;

    for(std::size_t i = 0; i < max && i < c.segments[index].size(); ++i)
    {
        v.push_back(c.segments[index][i]);
    }

    if(index < c.dm.size())
    {
        for(auto &i: c.dm[index].comments)
        {
            std::cout << i.text << "\n";
        }
    }

    std::cout << pcx::join_str(v, ",", [](char c){ return pcx::str(int(static_cast<unsigned char>(c))); }) << (c.segments[index].size() > max ? "..." : "") << "\n";
}

void processFunction(Context &c, std::size_t index)
{
    auto &sg = c.segments[index];

    ObjectDisassembler od(index);
    od.disassemble(c, std::cout, sg.data(), sg.size());
}

}

void Process::processUnit(Context &c, const std::string &path)
{
    c.unit = Loader::loadObjectUnit(path, c.segments);

    auto &u = c.unit;

    std::cout << banner(path, " stringtable");

    auto pw = padw(u.strings.size());
    for(auto s: pcx::indexed_range(u.strings))
    {
        std::cout << pad(s.index, pw) << ": " << s.value << "\n";
    }

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

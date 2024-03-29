#include "Process.h"

#include "framework/Console.h"
#include "framework/LoadBinaryFile.h"

#include "application/Context.h"
#include "application/Loader.h"

#include "disassembler/Disassembler.h"

#include <pcx/join_str.h>
#include <pcx/str.h>
#include <pcx/indexed_range.h>
#include <pcx/optional.h>

namespace
{

void outputValue(Context &c, std::size_t index, pcx::optional<std::size_t> address, std::vector<char> v)
{
    std::size_t max = 20;
    bool reduced = false;

    if(v.size() > max)
    {
        v.resize(max);
        reduced = true;
    }

    if(index < c.dm.size())
    {
        for(auto &i: c.dm[index].comments)
        {
            std::cout << i.text << "\n";
        }
    }

    if(address)
    {
        std::cout << *address << ": ";
    }

    std::cout << pcx::join_str(v, ",", [](char c){ return pcx::str(int(static_cast<unsigned char>(c))); }) << (reduced ? "..." : "") << "\n";
}

void processFunction(Context &c, Object::Entity &entity, std::size_t index)
{
    Disassembler ds(index);
    ds.disassemble(c, std::cout, entity.data.data(), entity.data.size());
}

}

void Process::processUnit(Context &c, const std::string &path)
{
    c.unit = Loader::loadObjectUnit(path);

    auto &u = c.unit;

    std::cout << banner(path, " stringtable");

    auto pw = padw(u.strings.size());
    for(auto s: pcx::indexed_range(u.strings))
    {
        std::cout << pad(s.index, pw) << ": " << s.value << "\n";
    }

    for(auto e: pcx::indexed_range(u.entities))
    {
        std::cout << banner(u.strings[e.value.id], " flags[", static_cast<unsigned>(e.value.flags), "]");

        switch(e.value.type)
        {
            case 'V': outputValue(c, e.index, { }, e.value.data); break;
            case 'F': processFunction(c, e.value, e.index); break;
        }
    }
}

void Process::processExe(Context &c, const std::string &path)
{
    auto v = loadBinaryFile(path);

    std::cout << banner("disassemble ", path);

    Disassembler ds(0);
    ds.disassemble(c, std::cout, v.data(), v.size());
}

void Process::processMappedExe(Context &c, const std::string &path)
{
    auto data = loadBinaryFile(path);
    std::size_t pc = 0;

    std::cout << banner("disassemble ", path);

    for(auto d: pcx::indexed_range(c.dm))
    {
        if(!d.value.name.empty())
        {
            std::cout << banner(d.value.name);
        }

        if(d.value.type == 'V')
        {
            outputValue(c, d.index, pc, { data.data() + pc, data.data() + pc + d.value.size });
        }
        else if(d.value.type == 'F')
        {
            Disassembler ds(d.index, pc);
            ds.disassemble(c, std::cout, data.data() + pc, d.value.size);
        }

        pc += d.value.size;
    }
}

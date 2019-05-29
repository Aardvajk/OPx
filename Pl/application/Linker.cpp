#include "Linker.h"

#include "framework/Error.h"

#include "application/Context.h"

namespace
{

void linkEntity(Context &c, Unit &unit, Entity &entity)
{
    for(auto p: entity.links)
    {
        auto id = p.first;
        auto addr = p.second;

        auto ent = c.lookup(unit.strings[id]);
        if(!ent)
        {
            throw Error("symbol not found - ", unit.strings[id]);
        }

        auto link = c.ds.position() + ent->offset;

std::cout << "linking " << unit.strings[id] << " at " << entity.offset + addr << " with " << link << "\n";

        c.ps.writeAt(entity.offset + addr, &link, sizeof(std::size_t));
    }
}

}

void Linker::link(Context &c)
{
    for(auto &u: c.units)
    {
        for(auto &e: u.entities)
        {
            linkEntity(c, u, e);
        }
    }
}

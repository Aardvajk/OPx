#include "Linker.h"

#include "framework/Error.h"
#include "framework/Console.h"

#include "application/Context.h"

namespace
{

void linkEntity(Context &c, Object::Unit &unit, Object::Entity &entity)
{
    for(auto ln: entity.links)
    {
        auto name = unit.strings[ln.id];

        auto e = c.find(name);
        if(!e)
        {
            throw Error("unable to locate - ", name);
        }

        auto addr = c.ds.position() + e->offset;

        std::cout << "linking " << name << " to " << addr << "\n";

        c.ps.writeAt(ln.address + entity.offset, &addr, sizeof(std::size_t));
    }
}

}

void Linker::link(Context &c)
{
    std::cout << banner("linking");

    for(auto &unit: c.units)
    {
        for(auto &entity: unit.entities)
        {
            linkEntity(c, unit, entity);
        }
    }
}

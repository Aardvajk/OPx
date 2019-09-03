#include "Composor.h"

#include "application/Context.h"

#include <fstream>

#include <pcx/str.h>

void Composor::compose(Context &c)
{
    for(std::size_t u = 0; u < c.units.size(); ++u)
    {
        auto &unit = c.units[u];

        for(std::size_t e = 0; e < unit.entities.size(); ++e)
        {
            auto &ent = unit.entities[e];

            if(c.refs.find(unit.strings[ent.id]) != c.refs.end())
            {
                auto &bs = (ent.type == 'V' ? c.ds : c.ps);
                ent.offset = bs.position();

                bs.write(c.data[u][e].data(), c.data[u][e].size());
            }
        }
    }
}

#include "Context.h"

Context::Context()
{
}

pcx::optional<Entity> Context::lookup(const std::string &name)
{
    for(auto &u: units)
    {
        for(auto &e: u.entities)
        {
            if(u.strings[e.id] == name)
            {
                return e;
            }
        }
    }

    return { };
}

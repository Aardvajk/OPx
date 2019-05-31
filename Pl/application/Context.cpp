#include "Context.h"

Context::Context() : dataComments(pcx::make_callback(this, &getDataPosition))
{
}

std::size_t Context::getDataPosition() const
{
    return ds.position();
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

#include "GenericParams.h"

GenericParams::GenericParams()
{
}

bool GenericParams::namesEquivalent(const GenericParams &g) const
{
    return params.size() == g.params.size();
}

GenericParams GenericParams::combine(const std::vector<Type*> &types) const
{
    auto p = *this;
    for(std::size_t i = 0; i < types.size(); ++i)
    {
        p.params[i].type = types[i];
    }

    return p;
}

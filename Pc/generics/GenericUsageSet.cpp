#include "GenericUsageSet.h"

#include "types/TypeCompare.h"

namespace
{

bool same(Context &c, const GenericUsage &a, const GenericUsage &b)
{
    if(a.sym != b.sym)
    {
        return false;
    }

    if(a.types.size() != b.types.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.types.size(); ++i)
    {
        if(!TypeCompare(c).exact(a.types[i], b.types[i]))
        {
            return false;
        }
    }

    return true;
}

}

GenericUsageSet::GenericUsageSet()
{
}

void GenericUsageSet::insert(Context &c, const GenericUsage &u)
{
    for(auto i: v)
    {
        if(same(c, i, u))
        {
            return;
        }
    }

    v.push_back(u);
}

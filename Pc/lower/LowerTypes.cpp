#include "LowerTypes.h"

#include "application/Context.h"

#include "types/Type.h"

void LowerTypes::convertRefsToPtrs(Context &c)
{
    for(auto &t: c.types)
    {
        if(t.ref)
        {
            ++t.ptr;
        }
    }
}

void LowerTypes::removeRefs(Context &c)
{
    for(auto &t: c.types)
    {
        t.ref = false;
    }
}

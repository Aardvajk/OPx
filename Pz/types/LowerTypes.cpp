#include "LowerTypes.h"

#include "application/Context.h"

#include "types/Type.h"

void LowerTypes::lower(Context &c)
{
    for(auto &t: c.types)
    {
        if(t.ref)
        {
            ++t.ptr;
        }
    }
}

#include "TypeCompare.h"

#include "types/Type.h"

bool TypeCompare::compatible(const Type *a, const Type *b)
{
    if(a->ptr != b->ptr)
    {
        return false;
    }

    if(a->sym != b->sym)
    {
        return false;
    }

    if(a->args.size() != b->args.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!exact(a->args[i], b->args[i]))
        {
            return false;
        }
    }

    if((a->returnType && !b->returnType) || (b->returnType && !a->returnType))
    {
        return false;
    }

    if(!exact(a->returnType, b->returnType))
    {
        return false;
    }

    return true;
}

bool TypeCompare::exact(const Type *a, const Type *b)
{
    if(!compatible(a, b))
    {
        return false;
    }

    if(a->constant != b->constant || a->ref != b->ref)
    {
        return false;
    }

    return true;
}

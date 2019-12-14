#include "TypeCompare.h"

#include "types/Type.h"

namespace
{

bool compare(const std::vector<Type*> &a, const std::vector<Type*> &b, bool(*cmp)(const Type*, const Type*))
{
    if(a.size() == b.size())
    {
        for(std::size_t i = 0; i < a.size(); ++i)
        {
            if(!cmp(a[i], b[i]))
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

}

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

    if(a->returnType && b->returnType && !exact(a->returnType, b->returnType))
    {
        return false;
    }

    if(a->genericRef != b->genericRef)
    {
        return false;
    }

    return true;
}

bool TypeCompare::compatible(const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    return compare(a, b, compatible);
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

bool TypeCompare::exact(const std::vector<Type*> &a, const std::vector<Type*> &b)
{
    return compare(a, b, exact);
}

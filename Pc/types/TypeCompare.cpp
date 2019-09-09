#include "TypeCompare.h"

#include "types/Type.h"

namespace
{

bool args(const Type *a, const Type *b, bool(*cmp)(const Type*, const Type*))
{
    if(a->args.size() != b->args.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!cmp(a->args[i], b->args[i]))
        {
            return false;
        }
    }

    return true;
}

}

bool TypeCompare::compatible(const Type *a, const Type *b)
{
    if(!a && !b)
    {
        return true;
    }

    if(!a || !b)
    {
        return false;
    }

    if(a->sym != b->sym || a->ptr != b->ptr)
    {
        return false;
    }

    if(a->method != b->method)
    {
        return false;
    }

    if(!exact(a->returnType, b->returnType))
    {
        return false;
    }

    return exactArgs(a, b);
}

bool TypeCompare::compatibleArgs(const Type *a, const Type *b)
{
    return args(a, b, compatible);
}

bool TypeCompare::exact(const Type *a, const Type *b)
{
    if(a && b)
    {
        if(a->ref != b->ref)
        {
            return false;
        }

        if(a->constant != b->constant)
        {
            return false;
        }

        if(a->constMethod != b->constMethod)
        {
            return false;
        }
    }

    return compatible(a, b);
}

bool TypeCompare::exactArgs(const Type *a, const Type *b)
{
    return args(a, b, exact);
}

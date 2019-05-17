#include "TypeCompare.h"

#include "types/Type.h"

bool TypeCompare::exact(const Type *a, const Type *b)
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

    if(!exact(a->returnType.get(), b->returnType.get()))
    {
        return false;
    }

    return args(a, b);
}

bool TypeCompare::args(const Type *a, const Type *b)
{
    if(a->args.size() != b->args.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!exact(a->args.ptr(i), b->args.ptr(i)))
        {
            return false;
        }
    }

    return true;
}

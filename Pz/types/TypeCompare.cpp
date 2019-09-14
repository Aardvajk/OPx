#include "TypeCompare.h"

#include "types/Type.h"

TypeCompare::TypeCompare(Context &c) : c(c)
{
}

bool TypeCompare::compatible(const Type *a, const Type *b) const
{
    if(!a && !b)
    {
        return true;
    }

    if(!a || !b)
    {
        return false;
    }

    if(a->sym != b->sym)
    {
        return false;
    }

    if(!exact(a->returnType, b->returnType))
    {
        return false;
    }

    if(!exactArgs(a, b))
    {
        return false;
    }

    return true;
}

bool TypeCompare::compatibleArgs(const Type *a, const Type *b) const
{
    if(a->args.size() != b->args.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a->args.size(); ++i)
    {
        if(!compatible(a->args[i], b->args[i]))
        {
            return false;
        }
    }

    return true;
}

bool TypeCompare::exact(const Type *a, const Type *b) const
{
    return compatible(a, b);
}

bool TypeCompare::exactArgs(const Type *a, const Type *b) const
{
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

    return true;
}

bool TypeCompare::convertsTo(const Type *from, const Type *to) const
{
    return compatible(from, to);
}

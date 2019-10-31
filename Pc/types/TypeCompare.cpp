#include "TypeCompare.h"

#include "types/Type.h"
#include "types/TypeConvert.h"

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

    if(a->ptr != b->ptr)
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

    if(a->method != b->method)
    {
        return false;
    }

    if(a->constMethod != b->constMethod)
    {
        return false;
    }

    if(a->generic != b->generic)
    {
        return false;
    }

    return true;
}

bool TypeCompare::compatibleArgs(const std::vector<Type*> &a, const std::vector<Type*> &b) const
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < a.size(); ++i)
    {
        if(!compatible(a[i], b[i]))
        {
            return false;
        }
    }

    return true;
}

bool TypeCompare::exact(const Type *a, const Type *b) const
{
    if(!compatible(a, b))
    {
        return false;
    }

    if(a && b)
    {
        if(a->constant != b->constant)
        {
            return false;
        }

        if(a->ref != b->ref)
        {
            return false;
        }
    }

    return true;
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

bool TypeCompare::convertible(Type *from, Type *to) const
{
    return compatible(from, to) || !TypeConvert::find(c, from, to, TypeConvert::Permission::Implicit).empty();
}

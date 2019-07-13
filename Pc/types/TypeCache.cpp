#include "TypeCache.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include <iostream>

TypeCache::TypeCache()
{
}

const Type *TypeCache::insert(const Type &type)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(TypeCompare::exact(v.ptr(i), &type))
        {
            return v.ptr(i);
        }
    }

    v.push_back(new Type(type));
    return v.back_ptr();
}

const Type *TypeCache::nullType() const
{
    return v.ptr(0);
}

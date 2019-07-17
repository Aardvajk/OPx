#include "TypeCache.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include <iostream>

TypeCache::TypeCache()
{
}

TypeCache::~TypeCache()
{
}

Type *TypeCache::insert(const Type &type)
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

Type *TypeCache::nullType()
{
    return v.ptr(0);
}

#include "TypeCache.h"

#include "types/TypeCompare.h"

TypeCache::TypeCache()
{
}

const Type *TypeCache::insert(const Type *type)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(TypeCompare::exact(v.ptr(i), type))
        {
            return v.ptr(i);
        }
    }

    v.push_back(type->clone());
    return v.back_ptr();
}

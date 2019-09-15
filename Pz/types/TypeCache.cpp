#include "TypeCache.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

TypeCache::TypeCache()
{
}

TypeCache::~TypeCache()
{
}

Type *TypeCache::insert(Context &c, const Type &type)
{
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(TypeCompare(c).exact(v.ptr(i), &type))
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

Type *TypeCache::intType()
{
    return v.ptr(1);
}

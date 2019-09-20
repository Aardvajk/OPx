#include "TypeCache.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

TypeCache::TypeCache(Context &c) : c(c)
{
}

TypeCache::~TypeCache()
{
}

Type *TypeCache::insert(const Type &type)
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

Type *TypeCache::charType()
{
    return v.ptr(1);
}

Type *TypeCache::intType()
{
    return v.ptr(2);
}

Type *TypeCache::sizeType()
{
    return v.ptr(3);
}

Type *TypeCache::boolType()
{
    return v.ptr(4);
}

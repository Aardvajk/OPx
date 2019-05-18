#ifndef TYPECACHE_H
#define TYPECACHE_H

#include "types/Type.h"

#include <pcx/ptr_vector.h>

class TypeCache
{
public:
    TypeCache();

    const Type *insert(const Type *type);
    const Type *insert(const Type &type);

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

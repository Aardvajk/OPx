#ifndef TYPECACHE_H
#define TYPECACHE_H

#include <pcx/ptr_vector.h>

class Type;

class TypeCache
{
public:
    TypeCache();

    const Type *insert(const Type &type);

    const Type *nullType() const;

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

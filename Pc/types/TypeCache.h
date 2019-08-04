#ifndef TYPECACHE_H
#define TYPECACHE_H

#include <pcx/ptr_vector.h>

class Type;

class TypeCache
{
public:
    TypeCache();
    ~TypeCache();

    Type *insert(const Type &type);

    Type *nullType();
    Type *intType();
    Type *boolType();
    Type *sizeType();

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

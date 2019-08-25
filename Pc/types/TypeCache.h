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
    Type *charType();
    Type *intType();
    Type *boolType();
    Type *sizeType();

    pcx::ptr_vector<Type>::iterator begin(){ return v.begin(); }
    pcx::ptr_vector<Type>::iterator end(){ return v.end(); }

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

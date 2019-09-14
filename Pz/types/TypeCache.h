#ifndef TYPECACHE_H
#define TYPECACHE_H

#include <pcx/ptr_vector.h>

class Context;
class Type;

class TypeCache
{
public:
    TypeCache();
    ~TypeCache();

    Type *insert(Context &c, const Type &type);

    Type *nullType();

    pcx::ptr_vector<Type>::iterator begin(){ return v.begin(); }
    pcx::ptr_vector<Type>::iterator end(){ return v.end(); }

private:
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

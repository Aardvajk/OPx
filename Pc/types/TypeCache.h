#ifndef TYPECACHE_H
#define TYPECACHE_H

#include <pcx/ptr_vector.h>

class Context;
class Type;

class TypeCache
{
public:
    TypeCache(Context &c);
    ~TypeCache();

    Type *insert(const Type &type);

    Type *nullType();
    Type *charType();
    Type *intType();
    Type *sizeType();
    Type *boolType();

    pcx::ptr_vector<Type>::iterator begin(){ return v.begin(); }
    pcx::ptr_vector<Type>::iterator end(){ return v.end(); }

private:
    Context &c;
    pcx::ptr_vector<Type> v;
};

#endif // TYPECACHE_H

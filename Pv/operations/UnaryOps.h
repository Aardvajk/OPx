#ifndef UNARYOPS_H
#define UNARYOPS_H

#include "common/Primitive.h"

#include "framework/ByteReader.h"

#include "components/Stack.h"

namespace UnaryOps
{

struct Not { template<typename T> static T op(T v){ return !v; } };
struct Neg { template<typename T> static T op(T v){ return -v; } };

namespace Detail
{

template<typename T, typename V> void process(Stack &s)
{
    V v;

    s.pop(v);
    s.push(T::op(v));
}

}

template<typename T> void unaryOp(ByteReader &rm, Stack &s)
{
    Primitive::Type p;
    rm(p);

    switch(p)
    {
        case Primitive::Type::Char: Detail::process<T, char>(s); break;
        case Primitive::Type::Int: Detail::process<T, int>(s); break;
        case Primitive::Type::Size: Detail::process<T, std::size_t>(s); break;

        default: break;
    }
}

}

#endif // UNARYOPS_H

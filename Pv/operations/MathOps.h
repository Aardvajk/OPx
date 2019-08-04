#ifndef MATHOPS_H
#define MATHOPS_H

#include "common/Primitive.h"

#include "framework/ByteReader.h"

#include "components/Stack.h"

namespace MathOps
{

struct Add { template<typename T> static T op(T a, T b){ return a + b; } };
struct Sub { template<typename T> static T op(T a, T b){ return a - b; } };
struct Mul { template<typename T> static T op(T a, T b){ return a * b; } };

namespace Detail
{

template<typename T, typename V> void process(Stack &s)
{
    V a, b;

    s.pop(b);
    s.pop(a);

    s.push(T::op(a, b));
}

}

template<typename T> void mathOp(ByteReader &rm, Stack &s)
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

#endif // MATHOPS_H

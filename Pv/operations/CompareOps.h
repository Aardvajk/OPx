#ifndef COMPAREOPS_H
#define COMPAREOPS_H

#include "common/Primitive.h"

#include "framework/ByteReader.h"

#include "components/Stack.h"

namespace CompareOps
{

struct Lt { template<typename T> static bool op(T a, T b){ return a < b; } };
struct LtEq { template<typename T> static bool op(T a, T b){ return a <= b; } };

namespace Detail
{

template<typename T, typename V> void process(Stack &s)
{
    V a, b;

    s.pop(b);
    s.pop(a);

    s.push(static_cast<char>(T::op(a, b)));
}

}

template<typename T> void compareOp(ByteReader &rm, Stack &s)
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

#endif // COMPAREOPS_H

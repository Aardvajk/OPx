#include "TestOps.h"

#include "common/Primitive.h"

#include "framework/Error.h"
#include "framework/ByteReader.h"

#include "components/Stack.h"

namespace
{

template<typename S> void process(Stack &s)
{
    S v;

    s.pop(v);
    s.push(v ? char(1) : char(0));
}

Error internalError()
{
    return Error("internal error, primitive test failed");
}

}

void TestOps::test(ByteReader &rm, Stack &s)
{
    Primitive::Type v;
    rm(v);

    switch(v)
    {
        case Primitive::Type::Char: process<char>(s); break;
        case Primitive::Type::Int: process<int>(s); break;
        case Primitive::Type::Size: process<std::size_t>(s); break;

        default: throw internalError();
    }
}

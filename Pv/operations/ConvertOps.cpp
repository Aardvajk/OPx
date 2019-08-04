#include "ConvertOps.h"

#include "common/Primitive.h"

#include "framework/ByteReader.h"

#include "components/Stack.h"

namespace
{

template<typename S, typename D> void process(Stack &s)
{
    S v;

    s.pop(v);
    s.push(static_cast<D>(v));
}

}

void ConvertOps::convert(ByteReader &rm, Stack &s)
{
    Primitive::Type sv, dv;
    rm(sv, dv);

    switch(sv)
    {
        case Primitive::Type::Char:
        {
            switch(dv)
            {
                case Primitive::Type::Int: process<char, int>(s); break;
                case Primitive::Type::Size: process<char, std::size_t>(s); break;

                default: break;
            }
        }

        case Primitive::Type::Int:
        {
            switch(dv)
            {
                case Primitive::Type::Char: process<int, char>(s); break;
                case Primitive::Type::Size: process<int, std::size_t>(s); break;

                default: break;
            }
        }

        case Primitive::Type::Size:
        {
            switch(dv)
            {
                case Primitive::Type::Char: process<std::size_t, char>(s); break;
                case Primitive::Type::Int: process<std::size_t, int>(s); break;

                default: break;
            }
        }

        default: break;
    }
}

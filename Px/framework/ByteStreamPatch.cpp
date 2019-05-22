#include "ByteStreamPatch.h"

#include "framework/ByteStream.h"

ByteStreamPatch::ByteStreamPatch()
{
}

void ByteStreamPatch::patch(ByteStream &stream, std::size_t value)
{
    auto op = stream.o->tellp();

    stream.o->seekp(p);
    stream << value;

    stream.o->seekp(op);
}


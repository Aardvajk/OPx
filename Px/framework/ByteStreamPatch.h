#ifndef BYTESTREAMPATCH_H
#define BYTESTREAMPATCH_H

#include <iostream>

class ByteStream;

class ByteStreamPatch
{
public:
    ByteStreamPatch();

    void patch(ByteStream &stream, std::size_t value);

    std::size_t position() const { return p; }

private:
    friend class ByteStream;

    std::size_t p;
};

#endif // BYTESTREAMPATCH_H

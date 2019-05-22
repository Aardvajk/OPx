#ifndef BYTESTREAMPATCH_H
#define BYTESTREAMPATCH_H

#include <iostream>

class ByteStream;

class ByteStreamPatch
{
public:
    ByteStreamPatch();

    void patch(ByteStream &stream, std::size_t value);

private:
    friend class ByteStream;

    std::ostream::pos_type p;
};

#endif // BYTESTREAMPATCH_H

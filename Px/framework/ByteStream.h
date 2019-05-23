#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <iostream>
#include <sstream>

#include "framework/ByteStreamPatch.h"

class ByteStream
{
public:
    ByteStream();
    explicit ByteStream(std::ostream &o);

    ByteStream &operator<<(const std::string &s);
    ByteStream &operator<<(const char *s);
    ByteStream &operator<<(ByteStreamPatch &p);

    template<typename T> ByteStream &operator<<(const T &t){ o->write(reinterpret_cast<const char*>(&t), sizeof(T)); return *this; }

    void write(const void *data, std::size_t bytes);

    std::string data() const;
    std::size_t position() const;

private:
    friend class ByteStreamPatch;

    std::ostream *o;
    std::ostringstream os;
};

#endif // BYTESTREAM_H

#include "ByteStream.h"

ByteStream::ByteStream() : o(&os), os(std::ios::binary)
{
}

ByteStream::ByteStream(std::ostream &o) : o(&o)
{
}

ByteStream &ByteStream::operator<<(const std::string &s)
{
    (*this) << s.size();
    o->write(s.c_str(), s.size());

    return *this;
}

ByteStream &ByteStream::operator<<(const char *s)
{
    return (*this) << std::string(s);
}

ByteStream &ByteStream::operator<<(ByteStreamPatch &p)
{
    p.p = o->tellp();
    return (*this) << std::size_t(0);
}

void ByteStream::write(const void *data, std::size_t bytes)
{
    o->write(reinterpret_cast<const char*>(data), bytes);
}

std::string ByteStream::data() const
{
    return os.str();
}

std::size_t ByteStream::position() const
{
    return o->tellp();
}

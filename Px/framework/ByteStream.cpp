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
    o->write(s.c_str(), std::streamoff(s.size()));

    return *this;
}

ByteStream &ByteStream::operator<<(const char *s)
{
    return (*this) << std::string(s);
}

ByteStream &ByteStream::operator<<(ByteStreamPatch &p)
{
    p.p = std::size_t(o->tellp());
    return (*this) << std::size_t(0);
}

void ByteStream::write(const void *data, std::size_t bytes)
{
    o->write(reinterpret_cast<const char*>(data), std::streamoff(bytes));
}

void ByteStream::writeAt(std::size_t position, const void *data, std::size_t bytes)
{
    auto old = o->tellp();
    o->seekp(std::streamoff(position));

    write(data, bytes);

    o->seekp(old);
}

std::vector<char> ByteStream::data() const
{
    auto s = os.str();
    return std::vector<char>(s.begin(), s.end());
}

std::size_t ByteStream::position() const
{
    return std::size_t(o->tellp());
}

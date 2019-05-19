#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <sstream>

class ByteStream
{
public:
    ByteStream();

    template<typename T> ByteStream &operator<<(const T &v){ os.write(reinterpret_cast<const char*>(&v), sizeof(v)); return *this; }

    std::string str() const { return os.str(); }

private:
    std::ostringstream os;
};

#endif // BYTESTREAM_H

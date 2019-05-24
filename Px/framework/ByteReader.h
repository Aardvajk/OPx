#ifndef BYTEREADER_H
#define BYTEREADER_H

#include <cstring>

namespace detail
{

void get(const char *d, std::size_t &p)
{
}

template<typename T, typename... Args> void get(const char *d, std::size_t &p, T &t, Args&&... args)
{
    std::memcpy(reinterpret_cast<char*>(&t), d + p, sizeof(T));
    p += sizeof(T);

    get(d, p, std::forward<Args>(args)...);
}

template<typename T, typename... Args> void get(const char *d, std::size_t &p, Args&&... args)
{
    get(d, p, std::forward<Args>(args)...);
}

}

class ByteReader
{
public:
    ByteReader(const char *data, std::size_t &pc) : m(data), p(pc) { }

    template<typename... Args> ByteReader &operator()(Args&&... args){ detail::get(m, p, std::forward<Args>(args)...); return *this; }
    template<typename T> T get(){ T v; operator()(v); return v; }

private:
    const char *m;
    std::size_t &p;
};

#endif // BYTEREADER_H

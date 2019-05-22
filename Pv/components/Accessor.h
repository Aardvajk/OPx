#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <cstring>

namespace Accessor
{

namespace detail
{

inline void get(const char *m, std::size_t &p){ }

template<typename T, typename... Args> void get(const char *m, std::size_t &p, T &t, Args&... args)
{
    std::memcpy(reinterpret_cast<char*>(&t), m + p, sizeof(T));
    p += sizeof(T);

    get(m, p, args...);
}

}

class Reader
{
public:
    Reader(const char *m, std::size_t &p) : m(m), p(p) { }

    template<typename... Args> void operator()(Args&... args){ detail::get(m, p, args...); }

private:
    const char *m;
    std::size_t &p;
};

}

#endif // ACCESSOR_H

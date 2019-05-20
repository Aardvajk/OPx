#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <cstring>

namespace Accessor
{

namespace detail
{

void get(char *&p){ }

template<typename T, typename... Args> void get(char *&p, T &t, Args&... args)
{
    std::memcpy(reinterpret_cast<char*>(&t), p, sizeof(T));
    p += sizeof(T);

    get(p, args...);
}

}

class Reader
{
public:
    Reader(char *&p) : p(p) { }

    template<typename... Args> void operator()(Args&... args){ detail::get(p, args...); }

private:
    char *&p;
};

}

#endif // ACCESSOR_H

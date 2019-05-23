#include "InputStream.h"

#include <vector>

InputStream::InputStream(std::istream &is) :is(&is)
{
}

InputStream &InputStream::operator>>(std::string &s)
{
    auto n = get<std::size_t>();

    std::vector<char> v(n + 1);
    is->read(v.data(), n);

    v[v.size() - 1] = '\0';

    s = v.data();
    return *this;
}

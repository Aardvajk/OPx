#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <iostream>

class InputStream
{
public:
    InputStream(std::istream &is);

    InputStream &operator>>(std::string &s);
    template<typename T> InputStream &operator>>(T &t){ is->read(reinterpret_cast<char*>(&t), sizeof(T)); return *this; }

    template<typename T> T get(){ T v; (*this) >> v; return v; }

    void read(void *data, std::size_t &bytes);

private:
    std::istream *is;
};

#endif // INPUTSTREAM_H

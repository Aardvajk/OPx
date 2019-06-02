#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/ByteStream.h"

#include "common/Object.h"

class InputStream;

class Context
{
public:
    Context();

    Object::Entity *find(const std::string &text);

    ByteStream &stream(char type);

    std::size_t offset(char type);
    void readData(char type, InputStream &is);

    std::vector<Object::Unit> units;

    ByteStream ds;
    ByteStream ps;
};

#endif // CONTEXT_H
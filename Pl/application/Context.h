#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/ByteStream.h"

#include "common/Object.h"
#include "common/DebugMap.h"

#include <unordered_map>
#include <unordered_set>

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
    std::vector<std::vector<std::vector<char> > > data;

    std::unordered_set<std::string> refs;

    ByteStream ds;
    ByteStream ps;

    DebugMap vd;
    DebugMap pd;

    bool trim;
};

#endif // CONTEXT_H

#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/ByteStream.h"

#include "common/Object.h"
#include "common/DebugMap.h"

#include <pcx/args.h>

#include <unordered_map>
#include <unordered_set>

class InputStream;

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    Object::Entity *find(const std::string &text);

    ByteStream &stream(char type);

    pcx::args args;

    std::vector<Object::Unit> units;
    std::unordered_set<std::string> refs;

    std::vector<std::string> globalInits;

    ByteStream ds;
    ByteStream ps;

    DebugMap vd;
    DebugMap pd;
};

#endif // CONTEXT_H

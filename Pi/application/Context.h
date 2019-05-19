#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "application/ByteStream.h"

#include "symbols/SymStack.h"

class Context
{
public:
    Context();

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &text);

    ByteStream &curr(){ return funcs.back(); }

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    std::vector<ByteStream> funcs;
};

#endif // CONTEXT_H

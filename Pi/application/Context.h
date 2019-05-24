#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "application/Function.h"
#include "application/StringTable.h"

#include "symbols/SymStack.h"

class Context
{
public:
    Context();

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &text);

    ByteStream &curr(){ return funcs.back().bytes; }

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    StringTable strings;
    std::vector<Function> funcs;
};

#endif // CONTEXT_H

#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

class Context
{
public:
    Context();

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &name);

    SourceList sources;
    Scanner scanner;

    SymTree tree;
    TypeCache types;
};

#endif // CONTEXT_H

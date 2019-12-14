#ifndef CONTEXT_H
#define CONTEXT_H

#include <pcx/args.h>

#include "scanner/Scanner.h"
#include "scanner/SourceList.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "generics/GenericStack.h"

#include "info/ParseInfo.h"

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    void open(const std::string &path);

    pcx::args args;

    Scanner scanner;
    SourceList sources;

    SymTree tree;
    TypeCache types;
    GenericStack generics;

    ParseInfo parseInfo;
};

#endif // CONTEXT_H

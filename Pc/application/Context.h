#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "symbols/SymTree.h"

#include <vector>

class Context
{
public:
    Context();

    void open(const std::string &path);

    SourceList sources;
    Scanner scanner;

    SymTree tree;
};

#endif // CONTEXT_H

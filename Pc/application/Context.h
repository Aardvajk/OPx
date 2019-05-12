#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "symbols/SymTree.h"

#include <vector>

class Node;

class Context
{
public:
    Context();

    void open(const std::string &path);

    Sym *search(Node *name, Sym *limit = nullptr);
    Sym *find(Node *name, Sym *limit = nullptr);

    std::string assertUnique(Location location, const std::string &name) const;

    SourceList sources;
    Scanner scanner;

    SymTree tree;
};

#endif // CONTEXT_H

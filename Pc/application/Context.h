#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "symbols/SymTree.h"

#include "visitors/SymFinder.h"

class Node;

class Context
{
public:
    Context();

    void open(const std::string &path);

    Sym *search(SymFinder::Policy policy, Node *name);
    Sym *find(SymFinder::Policy policy, Node *name);

    Sym *searchLocal(const std::string &name) const;

    std::string assertUnique(Location location, const std::string &name) const;

    SourceList sources;
    Scanner scanner;

    SymTree tree;
};

#endif // CONTEXT_H

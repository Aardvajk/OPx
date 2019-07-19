#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

class Node;

class Context
{
public:
    Context();

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &name);

    std::string assertSimpleName(Node *node);
    std::string assertSimpleNameUnique(Node *node);

    std::size_t assertSize(Location location, const Type *type);

    SourceList sources;
    Scanner scanner;

    std::vector<Sym::Type> containers;

    SymTree tree;
    TypeCache types;
};

#endif // CONTEXT_H

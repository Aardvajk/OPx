#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "symbols/SymTree.h"

#include "types/TypeCache.h"

#include "visitors/SymFinder.h"

class Node;
class Type;

class Context
{
public:
    Context();

    void open(const std::string &path);

    Sym *search(SymFinder::Policy policy, Node *name);
    Sym *find(SymFinder::Policy policy, Node *name);

    Sym *matchFunction(SymFinder::Policy policy, Node *name, const Type *type);

    Sym *searchLocal(const std::string &name) const;

    std::string assertUnique(Location location, const std::string &name) const;

    SourceList sources;
    Scanner scanner;

    SymTree tree;
    TypeCache types;
};

#endif // CONTEXT_H

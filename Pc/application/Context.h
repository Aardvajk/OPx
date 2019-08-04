#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

class Node;
class FuncNode;

class Context
{
public:
    Context();

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &name);

    std::string assertSimpleName(Node *node);
    std::string assertSimpleNameUnique(Node *node);

    std::size_t assertSize(Location location, const Type *type);
    std::size_t assertInitSize(Location location, const Type *type);

    std::string nextLabel();

    SourceList sources;
    Scanner scanner;

    std::vector<Sym::Type> containers;

    SymTree tree;
    TypeCache types;

    unsigned classDepth;
    std::vector<FuncNode*> deferredMethods;

    std::unordered_map<std::string, Node*> globals;
    std::size_t labels;
};

#endif // CONTEXT_H

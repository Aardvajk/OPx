#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include <pcx/args.h>

#include <unordered_map>

class Node;
class FuncNode;

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    void open(const std::string &path);

    void assertUnique(Location location, const std::string &name);

    std::string assertSimpleName(Node *node);
    std::string assertSimpleNameUnique(Node *node);

    std::size_t assertSize(Location location, const Type *type);
    std::size_t assertInitSize(Location location, const Type *type);

    Sym *assertChainedSym(Location location, Sym *start, const std::vector<std::string> &names);

    std::string nextLabel();
    std::string nextLabelQuoted();

    bool option(const std::string &key, const std::string &value) const;

    std::vector<pcx::args> args;

    SourceList sources;
    Scanner scanner;

    std::vector<Sym::Type> containers;

    SymTree tree;
    TypeCache types;

    unsigned classDepth;
    std::vector<FuncNode*> deferredMethods;

    std::unordered_map<std::string, Node*> globals;
    std::size_t labels;
    std::size_t scopes;

    std::vector<std::vector<Node*> > destructs;

    std::unordered_map<const Sym*, std::vector<std::pair<std::string, const Type*> > > temps;
    std::vector<std::pair<std::string, Type*> > tempDestructs;

    bool refsLowered;
    std::size_t pragmaPushes;
};

#endif // CONTEXT_H

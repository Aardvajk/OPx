#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "info/FuncInfo.h"

#include <pcx/args.h>
#include <pcx/ptr_vector.h>

#include <unordered_map>

class Node;
class FuncNode;

class Context
{
public:
    Context(int argc, char *argv[], std::vector<std::string> &files);

    void open(const std::string &path);

    bool option(const std::string &key) const;
    bool option(const std::string &key, const std::string &value) const;

    std::vector<std::string> values(const std::string &key) const;

    std::vector<pcx::args> args;

    SourceList sources;
    Scanner scanner;

    SymTree tree;
    TypeCache types;

    std::vector<Sym::Type> containers;
    pcx::ptr_vector<FuncInfo> funcInfos;

    std::size_t classDepth;
    std::vector<FuncNode*> deferredMethods;

    std::unordered_map<std::string, Node*> globals;
};

#endif // CONTEXT_H

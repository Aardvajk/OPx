#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "nodes/Node.h"

#include "syms/SymTree.h"

#include "types/TypeCache.h"

#include "info/Access.h"
#include "info/FuncInfo.h"

#include "generics/GenericStack.h"
#include "generics/GenericUsageSet.h"

#include <pcx/args.h>
#include <pcx/ptr_vector.h>
#include <pcx/scoped_lock.h>

#include <unordered_map>

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
    std::vector<Access> access;

    pcx::ptr_vector<FuncInfo> funcInfos;

    GenericStack generics;
    GenericUsageSet genericUsages;
    pcx::lock instantiating;

    std::size_t classDepth;
    std::vector<FuncNode*> deferredMethods;

    std::vector<std::unordered_map<std::string, NodePtr> > globals;
    std::size_t globalId;

    Node *globalInit;
    Node *globalDestroy;

    std::size_t potentiallySkipped;
};

#endif // CONTEXT_H

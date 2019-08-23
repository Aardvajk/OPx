#ifndef CONTEXT_H
#define CONTEXT_H

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

#include "common/DebugMap.h"

#include "components/Global.h"
#include "components/Function.h"
#include "components/StringTable.h"

#include "symbols/SymStack.h"

class Context
{
public:
    Context();

    void open(const std::string &path);

    Token matchId(bool get);

    void assertUnique(Location location, const std::string &text);

    std::size_t funcPosition() const;
    Function &func(){ return funcs.back(); }

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    StringTable strings;
    std::vector<Global> globs;
    std::vector<Function> funcs;

    DebugMap vd;
    DebugMap pd;
};

#endif // CONTEXT_H

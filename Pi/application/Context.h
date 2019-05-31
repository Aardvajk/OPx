#ifndef CONTEXT_H
#define CONTEXT_H

#include "framework/Comments.h"

#include "scanner/SourceList.h"
#include "scanner/Scanner.h"

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

    Function &func(){ return funcs.back(); }
    std::size_t currentPosition() const { return funcs.back().bytes.position(); }

    SourceList sources;
    Scanner scanner;

    SymStack syms;

    StringTable strings;
    std::vector<Function> funcs;

    Comments comments;
};

#endif // CONTEXT_H

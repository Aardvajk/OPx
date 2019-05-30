#ifndef FUNCTION_H
#define FUNCTION_H

#include "framework/ByteStream.h"

#include "scanner/Token.h"

#include <vector>

class Sym;

class Function
{
public:
    Function(Sym *sym, std::size_t id) : sym(sym), id(id), args(0), locals(0) { }

    Sym *sym;
    std::size_t id;

    std::size_t args;
    std::size_t locals;

    ByteStream bytes;

    std::vector<std::pair<Token,ByteStreamPatch> > jmpPatches;
    std::vector<std::pair<std::size_t, std::size_t> > links;
};

#endif // FUNCTION_H

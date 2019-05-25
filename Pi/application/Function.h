#ifndef FUNCTION_H
#define FUNCTION_H

#include "framework/ByteStream.h"

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
};

#endif // FUNCTION_H

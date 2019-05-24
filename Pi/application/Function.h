#ifndef FUNCTION_H
#define FUNCTION_H

#include "framework/ByteStream.h"

class Sym;

class Function
{
public:
    Function(Sym *sym, std::size_t id) : sym(sym), id(id) { }

    Sym *sym;
    std::size_t id;

    ByteStream bytes;
};

#endif // FUNCTION_H

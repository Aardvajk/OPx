#ifndef GLOBAL_H
#define GLOBAL_H

#include "framework/ByteStream.h"

class Sym;

class Global
{
public:
    Global(Sym *sym, std::size_t id) : sym(sym), id(id) { }

    Sym *sym;
    std::size_t id;

    ByteStream bytes;
};

#endif // GLOBAL_H

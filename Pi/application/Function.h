#ifndef FUNCTION_H
#define FUNCTION_H

#include "framework/ByteStream.h"

class Sym;

class Function
{
public:
    explicit Function(Sym *sym) : sym(sym) { }

    Sym *sym;
    ByteStream bytes;
};

#endif // FUNCTION_H

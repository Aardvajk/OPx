#ifndef GENERICUSAGE_H
#define GENERICUSAGE_H

#include <vector>

class Sym;
class Type;

class GenericUsage
{
public:
    GenericUsage(Sym *sym, std::vector<Type*> types) : sym(sym), types(std::move(types)) { }

    Sym *sym;
    std::vector<Type*> types;
};

#endif // GENERICUSAGE_H

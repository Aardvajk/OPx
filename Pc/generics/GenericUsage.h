#ifndef GENERICUSAGE_H
#define GENERICUSAGE_H

#include "scanner/Location.h"

#include <vector>

class Sym;
class Type;

class GenericUsage
{
public:
    GenericUsage(Location location, Sym *sym, std::vector<Type*> types) : location(location), sym(sym), types(std::move(types)) { }

    Location location;
    Sym *sym;
    std::vector<Type*> types;
};

#endif // GENERICUSAGE_H

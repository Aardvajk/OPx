#ifndef TYPELOOKUP_H
#define TYPELOOKUP_H

#include "scanner/Location.h"

#include <vector>

class Context;
class Sym;
class Type;

namespace TypeLookup
{

Sym *findNewMethod(Context &c, Type *type, const std::vector<Type*> &args);

Sym *assertNewMethod(Context &c, Location location, Type *type, const std::vector<Type*> &args);

}

#endif // TYPELOOKUP_H

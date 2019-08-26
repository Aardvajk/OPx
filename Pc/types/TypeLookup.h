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
Sym *findDeleteMethod(Context &c, Type *type);

Sym *assertNewMethod(Context &c, Location location, Type *type, const std::vector<Type*> &args);
Sym *assertNewCopyMethod(Context &c, Location location, Type *type);
Sym *assertDeleteMethod(Context &c, Location location, Type *type);

}

#endif // TYPELOOKUP_H

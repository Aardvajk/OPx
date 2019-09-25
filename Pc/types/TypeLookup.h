#ifndef TYPELOOKUP_H
#define TYPELOOKUP_H

#include "scanner/Location.h"

class Context;
class Sym;
class Type;

namespace TypeLookup
{

Sym *findCopyMethod(Context &c, Type *type);
Sym *findDeleteMethod(Context &c, Type *type);

Sym *assertCopyMethod(Context &c, Location location, Type *type);
Sym *assertDeleteMethod(Context &c, Location location, Type *type);

}

#endif // TYPELOOKUP_H

#ifndef TYPELOOKUP_H
#define TYPELOOKUP_H

#include "scanner/Location.h"

class Sym;
class Type;

namespace TypeLookup
{

Sym *assertDeleteMethod(Location location, Type *type);

}

#endif // TYPELOOKUP_H

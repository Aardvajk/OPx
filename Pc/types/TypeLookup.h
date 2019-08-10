#ifndef TYPELOOKUP_H
#define TYPELOOKUP_H

#include <vector>

class Context;
class Sym;
class Type;

namespace TypeLookup
{

Sym *findNewMethod(Context &c, Type *type, const std::vector<Type*> &args);

}

#endif // TYPELOOKUP_H

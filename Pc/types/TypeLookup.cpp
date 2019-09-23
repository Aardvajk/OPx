#include "TypeLookup.h"

#include "framework/Error.h"

#include "types/Type.h"

#include "syms/Sym.h"

Sym *TypeLookup::assertDeleteMethod(Location location, Type *type)
{
    if(type->sym)
    {
        if(auto f = type->sym->child("delete"))
        {
            return f;
        }
    }

    throw Error(location, "no delete method - ", type->text());
}

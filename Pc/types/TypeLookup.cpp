#include "TypeLookup.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "syms/Sym.h"

Sym *TypeLookup::findCopyMethod(Context &c, Type *type)
{
    auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(type->sym)) });

    for(auto s: type->sym->children())
    {
        if(s->name() == "new")
        {
            auto ft = s->property<Type*>("type");
            if(TypeCompare(c).compatibleArgs(ft, &t))
            {
                return s;
            }
        }
    }

    return nullptr;
}

Sym *TypeLookup::findDeleteMethod(Context &c, Type *type)
{
    return type->sym->child("delete");
}

Sym *TypeLookup::assertCopyMethod(Context &c, Location location, Type *type)
{
    if(auto s = type->sym->property<Sym*>("cachedCopyMethod"))
    {
        return s;
    }

    throw Error(location, "no copy method - ", type->text());
}

Sym *TypeLookup::assertDeleteMethod(Context &c, Location location, Type *type)
{
    if(auto s = type->sym->property<Sym*>("cachedDeleteMethod"))
    {
        return s;
    }

    throw Error(location, "no delete method - ", type->text());
}

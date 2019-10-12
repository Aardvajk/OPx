#include "TypeLookup.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "syms/Sym.h"

namespace
{

Sym *testAccess(Context &c, Location location, Sym *sym)
{
    if(!sym->accessibleFrom(c.tree.current()))
    {
        throw Error(location, "not accessible - ", sym->funcname());
    }

    return sym;
}

}

Sym *TypeLookup::findCopyMethod(Context &c, Type *type)
{
    auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(type->sym)) });

    for(auto s: type->sym->children())
    {
        if(s->name() == "new")
        {
            if(TypeCompare(c).compatibleArgs(Type::nonDefaultArgs(s), t.args))
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
        return testAccess(c, location, s);
    }

    throw Error(location, "no copy method - ", type->text());
}

Sym *TypeLookup::assertDeleteMethod(Context &c, Location location, Type *type)
{
    if(auto s = type->sym->property<Sym*>("cachedDeleteMethod"))
    {
        return testAccess(c, location, s);
    }

    throw Error(location, "no delete method - ", type->text());
}

#include "TypeLookup.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

Sym *TypeLookup::findNewMethod(Context &c, Type *type, const std::vector<Type*> &args)
{
    if(type->sym)
    {
        auto ct = Type::makeFunction(0, c.types.nullType());
        ct.args = args;
        ct.method = true;

        auto tp = Type::makePrimary(0, type->sym);
        tp.ref = true;

        if(c.refsLowered)
        {
            ++tp.ptr;
        }

        ct.args.insert(ct.args.begin(), c.types.insert(tp));

        for(auto s: type->sym->children())
        {
            if(s->name() == "new")
            {
                if(TypeCompare::compatibleArgs(s->property<Type*>("type"), &ct))
                {
                    return s;
                }
            }
        }
    }

    return nullptr;
}

Sym *TypeLookup::findDeleteMethod(Context &c, Type *type)
{
    if(type->sym)
    {
        return type->sym->child("delete");
    }

    return nullptr;
}

Sym *TypeLookup::assertNewMethod(Context &c, Location location, Type *type, const std::vector<Type*> &args)
{
    auto r = findNewMethod(c, type, args);
    if(!r)
    {
        throw Error(location, "no new(", pcx::join_str(args, ", ", [](Type *t){ return t->text(); }), ") method found - ", type->text());
    }

    return r;
}

Sym *TypeLookup::assertDeleteMethod(Context &c, Location location, Type *type)
{
    auto r = findDeleteMethod(c, type);
    if(!r)
    {
        throw Error(location, "no delete method found - ", type->text());
    }

    return r;
}

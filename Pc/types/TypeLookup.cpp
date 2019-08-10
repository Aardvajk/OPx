#include "TypeLookup.h"

#include "application/Context.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "syms/Sym.h"

Sym *TypeLookup::findNewMethod(Context &c, Type *type, const std::vector<Type*> &args)
{
    if(type->sym)
    {
        auto ct = Type::makeFunction(0, c.types.nullType());
        ct.args = args;
        ct.method = true;

        auto tp = Type::makePrimary(1, type->sym);
        ct.args.insert(ct.args.begin(), &tp);

        for(auto s: type->sym->children())
        {
            if(s->name() == "new")
            {
                if(TypeCompare::args(s->property<Type*>("type"), &ct))
                {
                    return s;
                }
            }
        }
    }

    return nullptr;
}

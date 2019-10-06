#include "TypeConvert.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

void findConvertMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv)
{
    auto t = Type::makeFunction(c.types.nullType(), { from });

    for(auto s: to->sym->children())
    {
        if(s->type() == Sym::Type::Func && s->name() == "new")
        {
            if(TypeCompare(c).compatibleArgs(&t, s->property<Type*>("type")))
            {
                sv.push_back(s);
            }
        }
    }
}

}

std::vector<Sym*> TypeConvert::find(Context &c, Type *from, Type *to)
{
    std::vector<Sym*> sv;

    if(!from->function() && !to->function())
    {
        findConvertMethod(c, from, to, sv);
    }

    return sv;
}

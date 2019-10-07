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
        if(s->type() == Sym::Type::Func && s->name() == "new" && !s->findProperty("explicit").value<bool>())
        {
            if(TypeCompare(c).compatibleArgs(&t, s->property<Type*>("type")))
            {
                sv.push_back(s);
            }
        }
    }
}

void findOperatorMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv)
{
    for(auto s: from->sym->children())
    {
        if(s->type() == Sym::Type::Func)
        {
            auto op = s->findProperty("opType");

            if(op && TypeCompare(c).compatible(to, op.to<Type*>()) && !s->findProperty("explicit").value<bool>())
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
        findOperatorMethod(c, from, to, sv);
    }

    return sv;
}

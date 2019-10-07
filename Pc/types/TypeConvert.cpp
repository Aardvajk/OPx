#include "TypeConvert.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

void findConvertMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv, TypeConvert::Permission p)
{
    auto t = Type::makeFunction(c.types.nullType(), { from });

    for(auto s: to->sym->children())
    {
        if(s->type() == Sym::Type::Func && s->name() == "new" && (!s->findProperty("explicit").value<bool>() || p == TypeConvert::Permission::Explicit))
        {
            if(TypeCompare(c).compatibleArgs(&t, s->property<Type*>("type")))
            {
                sv.push_back(s);
            }
        }
    }
}

void findOperatorMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv, TypeConvert::Permission p)
{
    for(auto s: from->sym->children())
    {
        if(s->type() == Sym::Type::Func)
        {
            auto op = s->findProperty("opType");

            if(op && TypeCompare(c).compatible(to, op.to<Type*>()) && (!s->findProperty("explicit").value<bool>() || p == TypeConvert::Permission::Explicit))
            {
                sv.push_back(s);
            }
        }
    }
}

}

std::vector<Sym*> TypeConvert::find(Context &c, Type *from, Type *to, Permission permission)
{
    std::vector<Sym*> sv;

    if(!from->function() && !to->function())
    {
        findConvertMethod(c, from, to, sv, permission);
        findOperatorMethod(c, from, to, sv, permission);
    }

    return sv;
}

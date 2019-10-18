#include "TypeConvert.h"

#include "application/Context.h"

#include "nodes/IdNode.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/CommonDecorator.h"

namespace
{

void findConvertMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv, TypeConvert::Permission p)
{
    auto t = Type::makeFunction(c.types.nullType(), { from });

    for(auto s: to->sym->children())
    {
        if(s->type() == Sym::Type::Func && s->name() == "new" && (!s->findProperty("explicit").value<bool>() || p == TypeConvert::Permission::Explicit))
        {
            if(TypeCompare(c).compatibleArgs(Type::nonDefaultArgs(s), t.args))
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

void findFreeMethod(Context &c, Type *from, Type *to, std::vector<Sym*> &sv, TypeConvert::Permission p)
{
    NodePtr id(new IdNode({ }, { }, pcx::str("operator ", to->text())));
    id->setProperty("opType", to);

    auto t = Type::makeFunction(c.types.nullType(), { from });

    auto v = CommonDecorator::searchCallable(c, id.get(), &t);
    sv.insert(sv.end(), v.begin(), v.end());
}

}

std::vector<Sym*> TypeConvert::find(Context &c, Type *from, Type *to, Permission permission)
{
    std::vector<Sym*> sv;

    if(!from->function() && !to->function())
    {
        findConvertMethod(c, from, to, sv, permission);
        findOperatorMethod(c, from, to, sv, permission);
        findFreeMethod(c, from, to, sv, permission);
    }

    return sv;
}

bool TypeConvert::nullPtrConvert(Context &c, Type *from, Type *to)
{
    auto t = *c.types.nullType();
    t.ptr = 1;

    if(TypeCompare(c).compatible(from, &t) && to->ptr == 1)
    {
        return true;
    }

    if(TypeCompare(c).compatible(to, &t) && from->ptr == 1)
    {
        return true;
    }

    return false;
}

bool TypeConvert::canConvert(Context &c, Type *from, Type *to)
{
    if(nullPtrConvert(c, from, to))
    {
        return true;
    }

    if(!find(c, from, to, Permission::Implicit).empty())
    {
        return true;
    }

    return false;
}


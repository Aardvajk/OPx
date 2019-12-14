#include "Type.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

namespace
{

std::string toString(const Type *type)
{
    std::string s;

    if(type->constant)
    {
        s += "const ";
    }

    if(type->ref)
    {
        s += "ref ";
    }

    for(std::size_t i = 0; i < type->ptr; ++i)
    {
        s += "ptr ";
    }

    if(type->sym)
    {
        s += type->sym->fullname();
    }
    else if(type->returnType)
    {
        s += pcx::str("(", pcx::join_str(type->args, ",", toString), ")");
    }
    else if(type->genericRef)
    {
        s += pcx::str("<", type->genericRef->index, ",", type->genericRef->depth, ">");
    }

    if(type->returnType)
    {
        s += ":" + toString(type->returnType);
    }

    return s;
}

}

Type::Type() : constant(false), ref(false), ptr(0), sym(nullptr), returnType(nullptr)
{
}

std::string Type::text() const
{
    return toString(this);
}

Type Type::makePrimary(Sym *sym)
{
    Type t;
    t.sym = sym;

    return t;
}

Type Type::makePrimary(bool constant, bool ref, Sym *sym)
{
    Type t;

    t.constant = constant;
    t.ref = ref;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(Type *returnType, const std::vector<Type*> &args)
{
    Type t;
    t.args = args;
    t.returnType = returnType;

    return t;
}

Type Type::makeGeneric(const GenericRef &ref)
{
    Type t;
    t.genericRef = ref;

    return t;
}

#include "Type.h"

#include "syms/Sym.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

std::string toString(const Type *type)
{
    std::string s;
    for(unsigned i = 0; i < type->ptr; ++i)
    {
        s += "ptr ";
    }

    if(type->sym)
    {
        s += type->sym->fullname();
    }
    else
    {
        s += "(" + pcx::join_str(type->args, ",", toString) + ")";
    }

    if(type->returnType)
    {
        s += ":" + toString(type->returnType);
    }

    return s;
}

}

pcx::optional<std::size_t> Type::size() const
{
    if(ptr || returnType)
    {
        return sizeof(std::size_t);
    }

    if(sym)
    {
        if(auto p = sym->getProperty("size"))
        {
            return p.to<std::size_t>();
        }
    }

    return { };
}

std::string Type::text() const
{
    return toString(this);
}

Type Type::makePrimary(unsigned ptr, Sym *sym)
{
    Type t;

    t.ptr = ptr;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(unsigned ptr, Type *returnType)
{
    Type t;

    t.ptr = ptr;
    t.returnType = returnType;

    return t;
}

Type::Type() : ptr(0), sym(nullptr), returnType(nullptr)
{
}

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

Type::Type() : ptr(0), sym(nullptr), returnType(nullptr)
{
}

Type::Type(unsigned ptr, Sym *sym) : ptr(ptr), sym(sym), returnType(nullptr)
{
}

Type::~Type()
{
}

std::string Type::text() const
{
    return toString(this);
}

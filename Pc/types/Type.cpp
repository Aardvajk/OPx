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

    if(type->sub)
    {
        s += pcx::str("[", type->sub, "]");
    }

    if(type->method)
    {
        s += " <method>";
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

pcx::optional<std::size_t> Type::initSize() const
{
    if(sub)
    {
        auto dt = *this;
        --dt.ptr;

        if(auto s = dt.size())
        {
            return (*s) * sub;
        }

        return { };
    }

    if(auto s = size())
    {
        return (*s) * (sub ? sub : 1);
    }

    return { };
}

std::string Type::text() const
{
    return toString(this);
}

bool Type::function() const
{
    return returnType;
}

bool Type::primitive() const
{
    return returnType || ptr || (sym && sym->getProperty("primitive").value<bool>());
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

Type Type::removeSub(const Type &type)
{
    auto r = type;
    r.sub = 0;

    return r;
}

Type::Type() : ptr(0), sym(nullptr), returnType(nullptr), sub(0), method(false)
{
}

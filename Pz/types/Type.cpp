#include "Type.h"

#include "framework/Error.h"

#include "syms/Sym.h"

#include <pcx/join_str.h>

namespace
{

std::string toString(const Type *type)
{
    std::string s;

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

Type Type::makePrimary(Sym *sym)
{
    Type t;
    t.sym = sym;

    return t;
}

Type Type::makeFunction(Type *returnType)
{
    Type t;
    t.returnType = returnType;

    return t;
}

Type::Type() : sym(nullptr), returnType(nullptr)
{
}

std::string Type::text() const
{
    return toString(this);
}

pcx::optional<std::size_t> Type::size() const
{
    if(sym)
    {
        if(auto sz = sym->findProperty("size"))
        {
            return sz.to<std::size_t>();
        }
    }

    return { };
}

std::size_t Type::assertSize(Location location, const Type *type)
{
    if(auto s = type->size())
    {
        return *s;
    }

    throw Error(location, "use of forward-declared type - ", type->text());
}

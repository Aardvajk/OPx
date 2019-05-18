#include "Type.h"

#include "symbols/Sym.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

std::string toString(const Type &type)
{
    std::string s;
    for(unsigned i = 0; i < type.ptr; ++i)
    {
        s += "ptr ";
    }

    if(type.sym)
    {
        s += type.sym->fullname();
    }
    else
    {
        s += "func(" + pcx::join_str(type.args, ",", toString) + ")";
    }

    if(type.returnType)
    {
        s += ":" + toString(*type.returnType);
    }

    return s;
}

}

Type::Type(Sym *sym) : sym(sym), ptr(0)
{
}

Type *Type::clone() const
{
    auto r = new Type(sym);
    r->ptr = ptr;

    r->returnType = returnType ? returnType->clone() : nullptr;
    for(auto &a: args)
    {
        r->args.push_back(a.clone());
    }

    return r;
}

std::string Type::text() const
{
    return toString(*this);
}

pcx::optional<std::size_t> Type::size() const
{
    if(ptr)
    {
        return 8;
    }

    if(sym)
    {
        if(auto pr = sym->property("size"))
        {
            return pr.to<std::size_t>();
        }
    }

    return { };
}

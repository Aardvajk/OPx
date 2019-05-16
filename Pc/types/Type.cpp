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
        s += "func";
    }

    if(!type.args.empty())
    {
        s += "(" + pcx::join_str(type.args, ",", toString) + ")";
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

std::string Type::text() const
{
    return toString(*this);
}

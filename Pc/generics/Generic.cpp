#include "Generic.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

Generic::Generic(std::string name, Type *type) : name(std::move(name)), type(type)
{
}

std::string Generic::funcName(Context &c, Sym *sym, const std::vector<Type*> &types)
{
    auto t = c.generics.convert(c, sym->property<Type*>("type"));
    return pcx::str(sym->fullname(), "<", pcx::join_str(types, ",", [](const Type *t){ return t->text(); }), ">", t->text());
}

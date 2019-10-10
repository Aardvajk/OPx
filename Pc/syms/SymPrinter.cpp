#include "SymPrinter.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>

namespace
{

std::string type(const Sym *sym)
{
    if(sym->type() == Sym::Type::Var && sym->parent()->type() == Sym::Type::Func)
    {
        return "arg";
    }

    return Sym::toString(sym->type());
}

std::string access(const Sym *sym)
{
    switch(sym->access())
    {
        case Access::Public: return "public";
        case Access::Private: return "private";
    }

    return { };
}

bool hasScope(const Sym *sym)
{
    if(sym->type() == Sym::Type::Func || sym->type() == Sym::Type::Class)
    {
        return sym->findProperty("defined").value<bool>();
    }

    return sym->type() == Sym::Type::Namespace || sym->type() == Sym::Type::Scope;
}

void dump(Context &c, int tab, const Sym *sym, std::ostream &os)
{
    if(sym->type() == Sym::Type::Namespace)
    {
        if(sym->name() == "std" && c.option("debug", "suppress_std"))
        {
            return;
        }
    }

    if(sym->type() == Sym::Type::Func)
    {
        if(sym->property<bool>("autogen") && c.option("debug", "suppress_autogens"))
        {
            return;
        }
    }

    auto ts = std::string(std::size_t(tab * 4), ' ');

    os << ts << sym << " " << access(sym) << " " << type(sym) << " " << sym->fullname();

    if(auto s = sym->findProperty("size"))
    {
        os << " size(" << s.to<std::size_t>() << ")";
    }

    if(auto t = sym->findProperty("type"))
    {
        os << " -> " << t.to<Type*>()->text();
    }

    os << "\n";

    if(hasScope(sym))
    {
        os << ts << "{\n";

        for(auto s: sym->children())
        {
            dump(c, tab + 1, s, os);
        }

        os << ts << "}\n";
    }
}

}

void SymPrinter::print(Context &c, const Sym *root, std::ostream &os)
{
    dump(c, 0, root, os);
}

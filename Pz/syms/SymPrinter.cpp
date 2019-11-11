#include "SymPrinter.h"

#include "application/Context.h"

#include "syms/Sym.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

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

bool hasScope(const Sym *sym)
{
    return sym->type() == Sym::Type::Namespace || sym->type() == Sym::Type::Scope;
}

void dump(Context &c, int tab, const Sym *sym, std::ostream &os)
{
    auto ts = std::string(std::size_t(tab * 4), ' ');

    os << ts << type(sym) << " " << sym->fullname() << "\n";

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

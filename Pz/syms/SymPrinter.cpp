#include "SymPrinter.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>

namespace
{

bool hasScope(const Sym *sym)
{
    if(sym->type() == Sym::Type::Func || sym->type() == Sym::Type::Class)
    {
        return sym->findProperty("defined").value<bool>();
    }

    return sym->type() == Sym::Type::Namespace || sym->type() == Sym::Type::Scope;
}

void dump(int tab, const Sym *sym, std::ostream &os)
{
    auto ts = std::string(std::size_t(tab * 4), ' ');

    os << ts << Sym::toString(sym->type()) << " " << sym->fullname() << " [" << sym << "]";

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
            dump(tab + 1, s, os);
        }

        os << ts << "}\n";
    }
}

}

void SymPrinter::print(const Sym *root, std::ostream &os)
{
    dump(0, root, os);
}

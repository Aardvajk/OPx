#include "SymPrinter.h"

#include "symbols/Sym.h"

#include "types/Type.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

namespace
{

bool hasScope(const Sym *sym)
{
    if(sym->type() == Sym::Type::Class || sym->type() == Sym::Type::Func)
    {
        return sym->property("defined").value<bool>();
    }

    return sym->type() == Sym::Type::Namespace || sym->type() == Sym::Type::Scope;
}

std::string typeToString(const Sym *sym)
{
    if(sym->type() == Sym::Type::Var)
    {
        if(sym->property("argument").value<bool>())
        {
            return "arg";
        }
    }

    return Sym::toString(sym->type());
}

std::string attrs(Sym::Attrs attr)
{
    std::vector<std::string> v;

    if(attr & Sym::Attr::Public) v.push_back("public");
    if(attr & Sym::Attr::Private) v.push_back("private");

    return pcx::join_str(v, " ");
}

std::string prepend(const std::string &s)
{
    return pcx::str(s.empty() ? "" : " ", s);
}

std::string prependParen(const std::string &s)
{
    return pcx::str(s.empty() ? "" : " (", s, s.empty() ? "" : ")");
}

void dump(int tab, const Sym *sym, std::ostream &os)
{
    auto ts = std::string(std::size_t(tab * 4), ' ');

    os << ts;

    auto as = attrs(sym->attrs());
    if(!as.empty())
    {
        os << as << " ";
    }

    os << typeToString(sym) << " [" << sym << "]" << prepend(sym->fullname()) << prependParen(sym->name());

    if(auto pr = sym->property("proxy-scope"))
    {
        auto ps = pr.to<const Sym*>();
        os << " proxy" << prepend(ps->fullname()) << " [" << ps << "]";
    }

    if(auto pr = sym->property("proxy-type"))
    {
        auto pt = pr.to<const Type*>();
        os << " proxy " << pt->text() << " [" << pt << "]";
    }

    if(auto pr = sym->property("type"))
    {
        os << " type " << pr.to<const Type*>()->text();
    }

    if(auto pr = sym->property("size"))
    {
        os << " size " << pr.to<std::size_t>();
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

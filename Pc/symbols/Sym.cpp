#include "Sym.h"

#include <pcx/str.h>
#include <pcx/join_str.h>

#include <algorithm>

namespace
{

bool scope(Sym::Type type)
{
    static const std::vector<Sym::Type> v = { Sym::Type::Global, Sym::Type::Class, Sym::Type::Namespace };
    return std::find(v.begin(), v.end(), type) != v.end();
}

void dump(int tab, const Sym *sym, std::ostream &os)
{
    auto ts = std::string(tab * 4, ' ');

    os << ts << Sym::toString(sym->type()) << " [" << sym << "] " << sym->fullname() << " (" << sym->name() << ")\n";

    if(auto pr = sym->property("proxy"))
    {
        os << ts << "    proxy [" << pr.to<const Sym*>() << "]\n";
    }

    if(scope(sym->type()))
    {
        os << ts << "{\n";
    }

    for(auto s: sym->children())
    {
        dump(tab + 1, s, os);
    }

    if(scope(sym->type()))
    {
        os << ts << "}\n";
    }
}

}

Sym::Sym(Type type, Attrs attrs, Location location, std::string name) : t(type), a(attrs), n(location), s(std::move(name)), ps(nullptr)
{
}

Sym *Sym::add(Sym *sym)
{
    sym->ps = this;
    cs.push_back(sym);

    return sym;
}

Sym *Sym::resolved()
{
    if(auto pr = property("proxy"))
    {
        return pr.to<Sym*>()->resolved();
    }

    return this;
}

void Sym::setProperty(const std::string &name, pcx::any value)
{
    pm[name] = value;
}

std::string Sym::fullname() const
{
    if(!ps) return name();

    std::vector<std::string> v;

    auto s = this;
    while(s->ps)
    {
        v.insert(v.begin(), s->s);
        s = s->ps;
    }

    return pcx::join_str(v, ".");
}

pcx::any Sym::property(const std::string &name) const
{
    auto i = pm.find(name);
    return i == pm.end() ? pcx::any() : i->second;
}

void Sym::print(std::ostream &os) const
{
    dump(0, this, os);
}

const char *Sym::toString(Type type)
{
    static const char *s[] = { "global", "namespace", "class", "using", "using-scope", "(invalid)" };
    return s[static_cast<int>(type)];
}

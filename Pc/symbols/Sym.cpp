#include "Sym.h"

#include <pcx/join_str.h>

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

bool Sym::accessibleBy(const Sym *scope) const
{
    if(attrs() & Sym::Attr::Private)
    {
        auto s = scope;
        while(s)
        {
            if(s == parent())
            {
                return true;
            }

            s = s->parent();
        }

        return false;
    }

    return true;
}

pcx::any Sym::property(const std::string &name) const
{
    auto i = pm.find(name);
    return i == pm.end() ? pcx::any() : i->second;
}

const char *Sym::toString(Type type)
{
    static const char *s[] = { "namespace", "class", "using", "using-scope", "(invalid)" };
    return s[static_cast<int>(type)];
}

bool Sym::isImportableScope(Type type)
{
    return type == Type::Namespace || type == Type::Class;
}

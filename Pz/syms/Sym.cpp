#include "Sym.h"

#include <pcx/join_str.h>

Sym::Sym(Type type, Location location, std::string name) : t(type), n(location), s(name), ps(nullptr)
{
}

Sym::~Sym()
{
    for(auto s: cs)
    {
        delete s;
    }
}

Sym *Sym::add(Sym *sym)
{
    cs.push_back(sym);
    sym->ps = this;

    return sym;
}

Sym *Sym::child(const std::string &name)
{
    for(auto s: cs)
    {
        if(s->s == name)
        {
            return s;
        }
    }

    return nullptr;
}

Sym *Sym::container()
{
    auto s = this;

    while(s)
    {
        switch(s->t)
        {
            case Type::Namespace:
            case Type::Class:
            case Type::Func: return s;

            default: s = s->ps;
        }
    }

    return nullptr;
}

void Sym::setProperty(const std::string &key, const pcx::any &value)
{
    pm[key] = value;
}

pcx::any Sym::findProperty(const std::string &key) const
{
    auto i = pm.find(key);
    return i == pm.end() ? pcx::any() : i->second;
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

const char *Sym::toString(Type v)
{
    static const char *s[] = { "namespace", "class", "scope", "func", "var" };
    return s[static_cast<int>(v)];
}

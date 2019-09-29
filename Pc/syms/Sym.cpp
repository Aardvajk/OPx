#include "Sym.h"

#include "types/Type.h"

#include <pcx/join_str.h>

Sym::Sym(Type type, Location location, Access access, std::string name) : t(type), n(location), a(access), s(name), ps(nullptr)
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

Sym *Sym::insert(std::size_t pos, Sym *sym)
{
    cs.insert(cs.begin() + static_cast<long long>(pos), sym);
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

std::vector<std::string> Sym::names() const
{
    if(!ps)
    {
        return { name() };
    }

    std::vector<std::string> v;

    auto s = this;
    while(s->ps)
    {
        v.insert(v.begin(), s->s);
        s = s->ps;
    }

    return v;
}

std::string Sym::fullname() const
{
    return pcx::join_str(names(), ".");
}

std::string Sym::funcname() const
{
    return pcx::str(fullname(), property<::Type*>("type")->text());
}

const char *Sym::toString(Type v)
{
    static const char *s[] = { "namespace", "class", "scope", "func", "var" };
    return s[static_cast<int>(v)];
}

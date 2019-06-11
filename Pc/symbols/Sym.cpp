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

Sym *Sym::child(const std::string &name)
{
    for(auto s: children())
    {
        if(s->name() == name)
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
        switch(s->type())
        {
            case Type::Namespace:
            case Type::Class:
            case Type::Func: return s;

            default: s = s->parent();
        }
    }

    return nullptr;
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
    static const char *s[] = { "namespace", "primitive", "class", "using-scope", "scope", "var", "func", "(invalid)" };
    return s[static_cast<int>(type)];
}

bool Sym::isPrimaryScope(Type type)
{
    return type == Type::Namespace || type == Type::Class;
}

bool Sym::isType(Type type)
{
    return type == Type::Primitive || type == Type::Class;
}

Sym::Attrs Sym::defaultAttrs(Type type)
{
    switch(type)
    {
        case Type::Namespace: return Attr::Public;
        case Type::Class: return Attr::Private;

        default: return { };
    }
}


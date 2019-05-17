#ifndef SYM_H
#define SYM_H

#include "scanner/Location.h"

#include <pcx/flags.h>
#include <pcx/any.h>

#include <string>
#include <vector>
#include <unordered_map>

class Sym
{
public:
    enum class Type
    {
        Namespace, Class, Using, UsingScope, Scope, Var, Func, Invalid
    };

    enum class Attr
    {
        Public = 1,
        Private = 2
    };

    using Attrs = pcx::flags<Attr>;

    Sym(Type type, Attrs attrs, Location location, std::string name);

    Sym *add(Sym *sym);
    Sym *resolved();

    void setProperty(const std::string &name, pcx::any value);

    Type type() const { return t; }
    Attrs attrs() const { return a; }

    Location location() const { return n; }
    std::string name() const { return s; }

    std::string fullname() const;

    bool accessibleBy(const Sym *scope) const;

    Sym *parent(){ return ps; }
    const Sym *parent() const { return ps; }

    std::vector<Sym*> &children(){ return cs; }
    const std::vector<Sym*> &children() const { return cs; }

    pcx::any property(const std::string &name) const;

    static const char *toString(Type type);
    static bool isImportableScope(Type type);
    static Attrs defaultAttrs(Type type);

private:
    Type t;
    Attrs a;

    Location n;
    std::string s;

    Sym *ps;
    std::vector<Sym*> cs;

    std::unordered_map<std::string, pcx::any> pm;
};

template<> struct pcx_is_flag_enum<Sym::Attr> : std::true_type { };

#endif // SYM_H

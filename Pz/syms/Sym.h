#ifndef SYM_H
#define SYM_H

#include "framework/Error.h"

#include "scanner/Location.h"

#include <pcx/any.h>

#include <string>
#include <vector>
#include <unordered_map>

class Sym
{
public:
    enum class Type
    {
        Namespace,
        Class,
        Scope,
        Func,
        Var
    };

    Sym(Type type, Location location, std::string name);
    ~Sym();

    Sym *add(Sym *sym);
    Sym *insert(std::size_t pos, Sym *sym);

    Sym *child(const std::string &name);
    Sym *container();

    void setProperty(const std::string &key, const pcx::any &value);
    pcx::any findProperty(const std::string &key) const;

    Type type() const { return t; }
    Location location() const { return n; }
    std::string name() const { return s; }

    std::vector<std::string> names() const;

    std::string fullname() const;

    Sym *parent(){ return ps; }
    const Sym *parent() const { return ps; }

    std::vector<Sym*> children(){ return cs; }
    const std::vector<Sym*> children() const { return cs; }

    template<typename T> T property(const std::string &key) const;

    static const char *toString(Type v);

private:
    Type t;
    Location n;
    std::string s;

    Sym *ps;
    std::vector<Sym*> cs;

    std::unordered_map<std::string, pcx::any> pm;
};

template<typename T> T Sym::property(const std::string &key) const
{
    auto i = pm.find(key);
    if(i == pm.end())
    {
        throw Error(location(), "bad property read on ", toString(type()), " sym ", fullname(), " - ", key);
    }

    return i->second.to<T>();
}

#endif // SYM_H

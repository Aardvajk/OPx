#include "Context.h"

#include "framework/Error.h"

#include "nodes/Node.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

Context::Context() : scanner(Lexer::Mode::Pc)
{
    auto ns = tree.current()->add(new Sym(Sym::Type::Namespace, Sym::Attr::Public, { }, "std"));

    ns->add(new Sym(Sym::Type::Primitive, Sym::Attr::Public, { }, "null"))->setProperty("size", std::size_t(0));
    ns->add(new Sym(Sym::Type::Primitive, Sym::Attr::Public, { }, "char"))->setProperty("size", std::size_t(1));
    ns->add(new Sym(Sym::Type::Primitive, Sym::Attr::Public, { }, "int"))->setProperty("size", std::size_t(4));

    types.insert(Type(tree.root()->child("std")->child("null")));
}

void Context::open(const std::string &path)
{
    pcx::scoped_ptr<std::ifstream> is(new std::ifstream(path));
    if(!is->is_open())
    {
        throw Error(Location(), "unable to open - ", path);
    }

    scanner.push(new Source(sources.id(path), is.release()));
}

Sym *Context::search(SymFinder::Policy policy, Node *name)
{
    SymFinder sf(policy, tree.root(), tree.current());
    name->accept(sf);

    auto r = sf.result();
    if(r.size() > 1)
    {
        throw Error(name->location(), "ambiguous - ", NameVisitors::prettyName(name));
    }

    if(!r.empty() && !r.front().accessible)
    {
        throw Error(name->location(), "not accessible - ", r.front().sym->fullname());
    }

    return r.empty() ? nullptr : r.front().sym;
}

Sym *Context::find(SymFinder::Policy policy, Node *name)
{
    auto sym = search(policy, name);

    if(!sym)
    {
        throw Error(name->location(), "not found - ", NameVisitors::prettyName(name));
    }

    return sym;
}

Sym *Context::matchFunction(SymFinder::Policy policy, Node *name, const Type *type)
{
    SymFinder sf(policy, tree.root(), tree.current());
    name->accept(sf);

    return matchFunction(name->location(), sf.result(), type);
}

Sym *Context::matchFunction(Location location, const std::vector<SymResult> &syms, const Type *type)
{
    std::vector<SymResult> r;

    for(auto s:syms)
    {
        if(s.sym->type() != Sym::Type::Func)
        {
            throw Error(location, "function expected - ", s.sym->fullname());
        }

        if(TypeCompare::args(type, s.sym->property("type").to<const Type*>()))
        {
            r.push_back(s);
        }
    }

    if(r.size() > 1)
    {
        throw Error(location, "ambiguous - ", r.front().sym->fullname());
    }

    if(!r.empty() && !r.front().accessible)
    {
        throw Error(location, "not accessible - ", r.front().sym->fullname());
    }

    return r.empty() ? nullptr : r.front().sym;
}

Sym *Context::searchLocal(const std::string &name) const
{
    for(auto s: tree.current()->children())
    {
        if(s->name() == name)
        {
            return s;
        }
    }

    return nullptr;
}

std::string Context::assertUnique(Location location, const std::string &name) const
{
    if(searchLocal(name))
    {
        throw Error(location, "already defined - ", name);
    }

    return name;
}

std::size_t Context::assertSize(Location location, const Type *type) const
{
    if(auto s = type->size())
    {
        return *s;
    }

    throw Error(location, "use of ", type->function() ? "function" : "incomplete", " type - ", type->text());
}

const Type *Context::identifyType(Node *node)
{
    TypeVisitor tn(*this);
    node->accept(tn);

    if(!tn.result())
    {
        throw Error(node->location(), "type expected - ", NameVisitors::prettyName(node));
    }

    return tn.result();
}

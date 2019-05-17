#include "Context.h"

#include "application/Error.h"

#include "nodes/Node.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

Context::Context()
{
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
        throw Error(name->location(), "ambiguous - ", name->text());
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
        throw Error(name->location(), "not found - ", name->text());
    }

    return sym;
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

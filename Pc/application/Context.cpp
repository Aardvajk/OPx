#include "Context.h"

#include "application/Error.h"

#include "nodes/Node.h"

#include "visitors/SymFinder.h"

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

Sym *Context::search(Node *name)
{
    SymFinder sf(tree.current());
    name->accept(sf);

    std::vector<Sym*> r;
    for(auto s: sf.result())
    {
        r.push_back(s->resolved());
    }

    if(r.size() > 1)
    {
        throw Error(name->location(), "ambiguous - ", name->text());
    }

    return r.empty() ? nullptr : r.front();
}

Sym *Context::find(Node *name)
{
    auto sym = search(name);

    if(!sym)
    {
        throw Error(name->location(), "not found - ", name->text());
    }

    return sym;
}

std::string Context::assertUnique(Location location, const std::string &name) const
{
    for(auto s: tree.current()->children())
    {
        if(s->name() == name)
        {
            throw Error(location, "already defined - ", name);
        }
    }

    return name;
}

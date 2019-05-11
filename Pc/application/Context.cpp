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

    auto r = sf.result();

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

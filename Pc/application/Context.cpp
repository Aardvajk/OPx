#include "Context.h"

#include "application/Error.h"

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

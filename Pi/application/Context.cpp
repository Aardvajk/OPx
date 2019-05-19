#include "Context.h"

#include "error/Error.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

Context::Context() : scanner(Lexer::Mode::Pi)
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

void Context::assertUnique(Location location, const std::string &text)
{
    if(syms.findLocal(text))
    {
        throw Error(location, "already defined - ", text);
    }
}

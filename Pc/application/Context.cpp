#include "Context.h"

#include "framework/Error.h"

#include "types/DefaultTypes.h"

#include <fstream>
#include <algorithm>

Context::Context(int argc, char *argv[], std::vector<std::string> &files) : scanner(Lexer::Mode::Pc), types(*this), classDepth(0)
{
    args.push_back({ argc, argv, files });

    DefaultTypes::create(*this);
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

bool Context::option(const std::string &key) const
{
    return args.back().contains(key);
}

bool Context::option(const std::string &key, const std::string &value) const
{
    if(args.back().contains(key))
    {
        auto v = args.back()[key];
        if(std::find(v.begin(), v.end(), value) != v.end())
        {
            return true;
        }
    }

    return false;
}

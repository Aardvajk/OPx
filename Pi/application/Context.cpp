#include "Context.h"

#include "framework/Error.h"

#include "common/OpCode.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

Context::Context() : scanner(Lexer::Mode::Pi)
{
}

Token Context::matchId(bool get)
{
    auto tok = scanner.next(get);
    if(tok.type() != Token::Type::Id && tok.type() != Token::Type::StringLiteral)
    {
        throw Error(tok.location(), "id expected - ", tok.text());
    }

    return tok;
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

Sym *Context::find(Location location, const std::string &name)
{
    if(auto s = syms.find(name))
    {
        return s;
    }

    throw Error(location, "not found - ", name);
}

std::size_t Context::funcPosition() const
{
    return funcs.back().bytes.position();
}


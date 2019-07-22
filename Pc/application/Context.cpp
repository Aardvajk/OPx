#include "Context.h"

#include "framework/Error.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

namespace
{

Sym *primitive(const std::string &name, std::size_t size)
{
    auto s = new Sym(Sym::Type::Class, {  }, name);

    s->setProperty("size", size);
    s->setProperty("primitive", true);

    return s;
}

}

Context::Context() : scanner(Lexer::Mode::Pc), classDepth(0)
{
    auto ns = tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(primitive("null", 0));
    ns->add(primitive("char", 1));
    ns->add(primitive("int", 4));
    ns->add(primitive("bool", 1));

    types.insert(Type::makePrimary(0, ns->child("null")));
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

void Context::assertUnique(Location location, const std::string &name)
{
    for(auto s: tree.current()->children())
    {
        if(s->name() == name)
        {
            throw Error(location, "already defined - ", name);
        }
    }
}

std::string Context::assertSimpleName(Node *node)
{
    if(!NameVisitors::isNameSimple(node))
    {
        throw Error(node->location(), "simple name expected - ", NameVisitors::prettyName(node));
    }

    return NameVisitors::lastIdOfName(node);
}

std::string Context::assertSimpleNameUnique(Node *node)
{
    auto name = assertSimpleName(node);
    assertUnique(node->location(), name);

    return name;
}

std::size_t Context::assertSize(Location location, const Type *type)
{
    auto sz = type->size();
    if(!sz)
    {
        throw Error(location, "undefined type used - ", type->text());
    }

    return *sz;
}

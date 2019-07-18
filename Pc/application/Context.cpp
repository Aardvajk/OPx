#include "Context.h"

#include "framework/Error.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

Context::Context() : scanner(Lexer::Mode::Pc)
{
    auto ns = tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(new Sym(Sym::Type::Class, { }, "null"))->setProperty("size", std::size_t(0));
    ns->add(new Sym(Sym::Type::Class, { }, "char"))->setProperty("size", std::size_t(1));
    ns->add(new Sym(Sym::Type::Class, { }, "int"))->setProperty("size", std::size_t(4));
    ns->add(new Sym(Sym::Type::Class, { }, "bool"))->setProperty("size", std::size_t(1));

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

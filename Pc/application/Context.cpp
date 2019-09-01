#include "Context.h"

#include "framework/Error.h"

#include "common/Primitive.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "nodes/Node.h"

#include "visitors/NameVisitors.h"

#include <pcx/scoped_ptr.h>

#include <fstream>

namespace
{

Sym *primitive(const std::string &name, Primitive::Type type, std::size_t size)
{
    auto s = new Sym(Sym::Type::Class, {  }, name);

    s->setProperty("size", size);
    s->setProperty("primitive", true);
    s->setProperty("primitiveType", type);

    return s;
}

void addPrimitive(Context &c, const std::string &name, Primitive::Type type, std::size_t size)
{
    auto s = c.tree.root()->child("std")->add(primitive(name, type, size));
    auto t = c.types.insert(Type::makePrimary(0, s));

    s->setProperty("type", t);
}

}

Context::Context(int argc, char *argv[], std::vector<std::string> &files)
    : scanner(Lexer::Mode::Pc), classDepth(0), labels(0), scopes(0), refsLowered(false)
{
    args.push_back({ argc, argv, files });

    tree.current()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    addPrimitive(*this, "null", Primitive::Type::Null, 0);
    addPrimitive(*this, "char", Primitive::Type::Char, 1);
    addPrimitive(*this, "int", Primitive::Type::Int, 4);
    addPrimitive(*this, "bool", Primitive::Type::Char, 1);
    addPrimitive(*this, "size", Primitive::Type::Size, 8);
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
    if(!name.empty())
    {
        for(auto s: tree.current()->children())
        {
            if(s->name() == name)
            {
                throw Error(location, "already defined - ", name);
            }
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

std::size_t Context::assertInitSize(Location location, const Type *type)
{
    auto sz = type->initSize();
    if(!sz)
    {
        throw Error(location, "undefined type used - ", type->text());
    }

    return *sz;
}

std::string Context::nextLabel()
{
    return pcx::str("#label_", labels++);
}

std::string Context::nextLabelQuoted()
{
    return pcx::str("\"", nextLabel(), "\"");
}

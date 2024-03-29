#include "DefaultTypes.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "types/Type.h"

#include <pcx/scoped_ptr.h>

namespace
{

Sym *createPrimitive(Context &c, const std::string &name, Primitive::Type type, std::size_t size)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, Access::Public, name);

    s->setProperty("size", size);
    s->setProperty("type", c.types.insert(Type::makePrimary(s.get())));
    s->setProperty("primitive", type);
    s->setProperty("complete", true);

    return s.release();
}

}

void DefaultTypes::create(Context &c)
{
    auto ns = c.tree.root()->add(new Sym(Sym::Type::Namespace, { }, Access::Public, "std"));

    ns->add(createPrimitive(c, "null", Primitive::Type::Null, 0));
    ns->add(createPrimitive(c, "char", Primitive::Type::Char, 1));
    ns->add(createPrimitive(c, "int", Primitive::Type::Int, 4));
    ns->add(createPrimitive(c, "size", Primitive::Type::Size, 8));
    ns->add(createPrimitive(c, "bool", Primitive::Type::Char, 1));
}

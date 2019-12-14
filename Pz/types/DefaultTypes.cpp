#include "DefaultTypes.h"

#include "common/Primitive.h"

#include "application/Context.h"

#include "types/Type.h"

#include <pcx/scoped_ptr.h>

namespace
{

Sym *createPrimitive(Context &c, const std::string &name, Primitive::Type type, std::size_t size)
{
    pcx::scoped_ptr<Sym> s = new Sym(Sym::Type::Class, { }, name);

    s->setProperty("type", c.types.insert(Type::makePrimary(s.get())));

    return s.release();
}

}

void DefaultTypes::create(Context &c)
{
    auto ns = c.tree.root()->add(new Sym(Sym::Type::Namespace, { }, "std"));

    ns->add(createPrimitive(c, "null", Primitive::Type::Null, 0));
    ns->add(createPrimitive(c, "int", Primitive::Type::Int, 4));
}

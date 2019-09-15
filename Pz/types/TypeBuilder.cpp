#include "TypeBuilder.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "types/Type.h"

#include "visitors/SymFinder.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(TypeNode &node)
{
    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

    if(sv.empty() || sv.front()->type() != Sym::Type::Class)
    {
        throw Error(node.name->location(), "type expected - ", node.name->description());
    }

    auto t = Type::makePrimary(sv.front());

    t.constant = node.constant;
    t.ref = node.ref;
    t.ptr = node.ptr;

    r = c.types.insert(c, t);
}

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
    Type t;

    if(node.function)
    {
        t = Type::makeFunction(node.returnType ? Visitor::query<TypeBuilder, Type*>(node.returnType.get(), c) : c.types.nullType());
        for(auto &a: node.args)
        {
            t.args.push_back(Visitor::query<TypeBuilder, Type*>(a.get(), c));
        }
    }
    else
    {
        std::vector<Sym*> sv;
        SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

        if(sv.empty() || sv.front()->type() != Sym::Type::Class)
        {
            throw Error(node.name->location(), "type expected - ", node.name->description());
        }

        if(!sv.front()->accessibleFrom(c.tree.current()))
        {
            throw Error(node.location(), "not accessible - ", node.description());
        }

        t = Type::makePrimary(sv.front());
    }

    t.constant = node.constant;
    t.ref = node.ref;
    t.ptr = node.ptr;

    r = c.types.insert(t);
}

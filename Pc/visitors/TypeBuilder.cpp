#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), t(new Type())
{
}

TypeBuilder::~TypeBuilder()
{
}

Type *TypeBuilder::claim()
{
    return t.release();
}

void TypeBuilder::visit(TypeNode &node)
{
    t->ptr = node.ptr;

    if(node.function)
    {
        for(auto &a: node.args)
        {
            TypeBuilder tb(c);
            a.accept(tb);

            t->args.push_back(tb.claim());
        }

        TypeBuilder tb(c);
        node.returnType->accept(tb);

        t->returnType = tb.claim();
    }
    else if(node.name)
    {
        SymFinder sf(c.tree.current());
        node.name->accept(sf);

        auto r = sf.result();
        if(r.empty() || r.front()->type() != Sym::Type::Class)
        {
            throw Error(node.name->location(), "type expected - ", NameVisitors::prettyName(node.name.get()));
        }

        t->sym = r.front();
    }
}

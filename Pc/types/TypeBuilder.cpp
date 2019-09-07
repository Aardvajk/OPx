#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"
#include "visitors/ArraySizeVisitor.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(TypeNode &node)
{
    if(node.function)
    {
        auto t = Type::makeFunction(node.ptr, node.returnType ? type(c, node.returnType.get()) : c.types.nullType());
        for(auto &a: node.args)
        {
            t.args.push_back(type(c, a.get()));
        }

        t.constant = node.constant;
        t.ref = node.ref;

        if(node.sub)
        {
            t.sub = ArraySizeVisitor::value(*node.sub);
            t.ptr = 1;
        }

        r = c.types.insert(t);
    }
    else
    {
        std::vector<Sym*> sv;
        SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), node.name.get(), sv);

        if(sv.empty() || sv.front()->type() != Sym::Type::Class)
        {
            throw Error(node.name->location(), "type expected - ", NameVisitors::prettyName(node.name.get()));
        }

        auto t = Type::makePrimary(node.ptr, sv.front());

        t.constant = node.constant;
        t.ref = node.ref;

        if(node.sub)
        {
            t.sub = ArraySizeVisitor::value(*node.sub);
            ++t.ptr;
        }

        r = c.types.insert(t);
    }
}

Type *TypeBuilder::type(Context &c, Node *node)
{
    TypeBuilder tb(c);
    node->accept(tb);

    return tb.result();
}

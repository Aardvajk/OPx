#include "TypeBuilder.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/VarNode.h"
#include "nodes/TypeNode.h"

#include "types/Type.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

TypeBuilder::TypeBuilder(Context &c) : c(c), r(nullptr)
{
}

void TypeBuilder::visit(VarNode &node)
{
    node.type->accept(*this);
}

void TypeBuilder::visit(TypeNode &node)
{
    if(node.function)
    {
        Type t(node.ptr, nullptr);
        t.returnType = node.returnType ? type(c, node.returnType.get()) : c.types.nullType();

        for(auto &a: node.args)
        {
            t.args.push_back(type(c, &a));
        }

        r = c.types.insert(t);
    }
    else
    {
        auto s = SymFinder::find(c.tree.current(), node.name.get());
        if(s.empty() || s.front()->type() != Sym::Type::Class)
        {
            throw Error(node.name->location(), "type expected - ", NameVisitors::prettyName(node.name.get()));
        }

        r = c.types.insert(Type(node.ptr, s.front()));
    }
}

const Type *TypeBuilder::type(Context &c, Node *node)
{
    TypeBuilder tb(c);
    node->accept(tb);

    return tb.result();
}

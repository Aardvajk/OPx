#include "QueryVisitors.h"

#include "nodes/IdNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

QueryVisitors::DirectType::DirectType() : r(nullptr)
{
}

void QueryVisitors::DirectType::visit(IdNode &node)
{
    if(node.property<Sym*>("sym")->type() == Sym::Type::Class)
    {
        r = node.property<Type*>("type");
    }
}

QueryVisitors::GetParent::GetParent()
{
}

void QueryVisitors::GetParent::visit(IdNode &node)
{
    r = node.parent;
}

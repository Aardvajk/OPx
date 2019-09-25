#include "QueryVisitors.h"

#include "nodes/IdNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

QueryVisitors::DirectType::DirectType() : r(nullptr)
{
}

void QueryVisitors::DirectType::visit(IdNode &node)
{
    auto sym = node.property<Sym*>("sym");
    if(sym->type() == Sym::Type::Class)
    {
        r = sym->property<Type*>("type");
    }
}

QueryVisitors::GetParent::GetParent()
{
}

void QueryVisitors::GetParent::visit(IdNode &node)
{
    r = node.parent;
}

QueryVisitors::GetConstructNode::GetConstructNode() : r(nullptr)
{
}

void QueryVisitors::GetConstructNode::visit(ConstructNode &node)
{
    r = &node;
}

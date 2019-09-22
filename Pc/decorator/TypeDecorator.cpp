#include "TypeDecorator.h"

#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"

#include "types/TypeBuilder.h"

TypeDecorator::TypeDecorator(Context &c) : c(c)
{
}

void TypeDecorator::visit(TypeNode &node)
{
    node.setProperty("type", Visitor::query<TypeBuilder, Type*>(&node, c));
}

void TypeDecorator::visit(VarNode &node)
{
    if(node.type)
    {
        node.type->accept(*this);
    }
}

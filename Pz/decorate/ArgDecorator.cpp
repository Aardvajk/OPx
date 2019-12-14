#include "ArgDecorator.h"

#include "application/Context.h"

#include "nodes/VarNode.h"

#include "types/TypeBuilder.h"

ArgDecorator::ArgDecorator(Context &c) : c(c)
{
}

void ArgDecorator::visit(VarNode &node)
{
    node.type->setProperty("type", Visitor::query<TypeBuilder, Type*>(node.type.get(), c));
}

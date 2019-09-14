#include "TypeVisitor.h"

#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"

TypeVisitor::TypeVisitor() : r(nullptr)
{
}

void TypeVisitor::visit(TypeNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(VarNode &node)
{
    r = node.type->property<Type*>("type");
}

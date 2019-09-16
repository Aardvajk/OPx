#include "TypeVisitor.h"

#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"

#include "types/Type.h"

TypeVisitor::TypeVisitor() : r(nullptr)
{
}

void TypeVisitor::visit(IdNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(TypeNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(VarNode &node)
{
    r = node.type->property<Type*>("type");
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(CallNode &node)
{
    r = node.property<Type*>("type")->returnType;
}

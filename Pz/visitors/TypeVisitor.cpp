#include "TypeVisitor.h"

#include "application/Context.h"

#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(TypeNode &node)
{
    r = node.property<Type*>("type");
}

void TypeVisitor::visit(VarNode &node)
{
    if(node.type)
    {
        r = node.type->property<Type*>("type");
    }
}

void TypeVisitor::visit(IntLiteralNode &node)
{
    r = c.types.intType();
}

Type *TypeVisitor::queryType(Context &c, Node *node)
{
    return Visitor::query<TypeVisitor, Type*>(node, c);
}

Type *TypeVisitor::assertType(Context &c, Node *node)
{
    auto t = queryType(c, node);
    if(!t)
    {
        throw Error(node->location(), "type visitor failed - ", node->classname(), "node ", node->description());
    }

    return t;
}

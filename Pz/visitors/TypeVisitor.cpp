#include "TypeVisitor.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/VarNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

TypeVisitor::TypeVisitor(Context &c) : c(c), r(nullptr)
{
}

void TypeVisitor::visit(IdNode &node)
{
    r = node.property<Sym*>("sym")->property<Type*>("type");
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
    r = c.types.intType();
}

void TypeVisitor::visit(CallNode &node)
{
    auto t = Visitor::query<TypeVisitor, Type*>(node.target.get(), c);
    r = t->returnType;
}

void TypeVisitor::visit(ConstructNode &node)
{
    r = node.type;
}

Type *TypeVisitor::queryType(Context &c, Node *node)
{
    return Visitor::query<TypeVisitor, Type*>(node, c);
}

Type *TypeVisitor::assertType(Context &c, Node *node)
{
    auto r = queryType(c, node);
    if(!r)
    {
        throw Error(node->location(), "type visitor failed - ", node->classname(), " node ", node->description());
    }

    return r;
}

#include "ExprLower.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"

#include "visitors/TypeVisitor.h"

#include "types/Type.h"

ExprLower::ExprLower(Context &c, NodePtr &cn, Type *expectedType) : c(c), cn(cn), expectedType(expectedType)
{
}

void ExprLower::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprLower::lower(c, node.parent);
    }

    auto type = TypeVisitor::assertType(c, &node);
    if(!type->ref)
    {
        if(expectedType && expectedType->ref)
        {
            rn = new AddrOfNode(node.location(), cn);
            rn->setProperty("type", c.types.insert(type->addPointer()));
        }
    }
}

void ExprLower::visit(CallNode &node)
{
    auto type = TypeVisitor::assertType(c, node.target.get());

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprLower::lower(c, node.params[i], type->args[i]);
    }
}

void ExprLower::visit(AddrOfNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

NodePtr ExprLower::lower(Context &c, NodePtr &node, Type *expectedType)
{
    ExprLower el(c, node, expectedType);
    node->accept(el);

    return el.result() ? el.result() : node;
}

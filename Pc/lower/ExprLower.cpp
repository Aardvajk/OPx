#include "ExprLower.h"

#include "nodes/IdNode.h"
#include "nodes/AssignNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"

ExprLower::ExprLower(Context &c, NodePtr &cn) : c(c), cn(cn)
{
}

void ExprLower::visit(IdNode &node)
{
    if(TypeVisitor::type(c, &node)->ref)
    {
        rn = new DerefNode(node.location(), cn);
    }
}

void ExprLower::visit(AssignNode &node)
{
    if(TypeVisitor::type(c, node.target.get())->ref && !TypeVisitor::type(c, node.expr.get())->ref)
    {
        node.expr = new AddrOfNode(node.expr->location(), node.expr);
    }
}

NodePtr ExprLower::lower(Context &c, NodePtr &node)
{
    ExprLower el(c, node);
    node->accept(el);

    return el.result() ? el.result() : node;
}

#include "ExprLower.h"

#include "nodes/AssignNode.h"
#include "nodes/AddrOfNode.h"

#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"

ExprLower::ExprLower(Context &c) : c(c)
{
}

void ExprLower::visit(AssignNode &node)
{
    if(TypeVisitor::type(c, node.target.get())->ref)
    {
        node.expr = new AddrOfNode(node.expr->location(), node.expr);
    }
}

NodePtr ExprLower::lower(Context &c, NodePtr &node)
{
    ExprLower el(c);
    node->accept(el);

    return el.result() ? el.result() : node;
}

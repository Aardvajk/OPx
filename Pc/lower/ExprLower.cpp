#include "ExprLower.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"

ExprLower::ExprLower(Context &c, NodePtr &cn, Flags flags) : c(c), cn(cn), flags(flags)
{
}

void ExprLower::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprLower::lower(c, node.parent);
    }

    if(!(flags & Flag::NoTopLevel) && TypeVisitor::type(c, &node)->ref)
    {
        rn = new DerefNode(node.location(), cn);
    }
}

void ExprLower::visit(CallNode &node)
{
    ExprLower::lower(c, node.params);
}

void ExprLower::visit(AssignNode &node)
{
    node.target = ExprLower::lower(c, node.target, Flag::NoTopLevel);
    node.expr = ExprLower::lower(c, node.expr, Flag::NoTopLevel);

    if(TypeVisitor::type(c, node.target.get())->ref && !TypeVisitor::type(c, node.expr.get())->ref)
    {
        node.expr = new AddrOfNode(node.expr->location(), node.expr);
    }
}

void ExprLower::visit(BinaryNode &node)
{
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

NodePtr ExprLower::lower(Context &c, NodePtr &node, Flags flags)
{
    ExprLower el(c, node, flags);
    node->accept(el);

    return el.result() ? el.result() : node;
}

void ExprLower::lower(Context &c, NodeList &nodes, Flags flags)
{
    for(std::size_t i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = ExprLower::lower(c, nodes[i], flags);
    }
}

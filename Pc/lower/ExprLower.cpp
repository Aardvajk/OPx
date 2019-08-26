#include "ExprLower.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeLookup.h"

ExprLower::ExprLower(Context &c, NodePtr &cn, const Type *type, Flags flags) : c(c), cn(cn), type(type), flags(flags)
{
}

void ExprLower::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprLower::lower(c, node.parent);
    }

    if(!(flags & Flag::NoTopLevel))
    {
        if(TypeVisitor::type(c, &node)->ref)
        {
            if(!type || !type->ref)
            {
                rn = new DerefNode(node.location(), cn);
            }
        }
        else if(type && type->ref)
        {
            rn = new AddrOfNode(node.location(), cn);
        }
    }
}

void ExprLower::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());
    if(!t->function())
    {
        t = node.target->property<Sym*>("newmethod")->property<Type*>("type");
    }

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprLower::lower(c, node.params[i], t->args[i]);
    }
}

void ExprLower::visit(AssignNode &node)
{
    node.target = ExprLower::lower(c, node.target, nullptr, Flag::NoTopLevel);
    node.expr = ExprLower::lower(c, node.expr, nullptr, Flag::NoTopLevel);

    if(node.getProperty("constructor").value<bool>())
    {
        if(TypeVisitor::type(c, node.target.get())->ref && !TypeVisitor::type(c, node.expr.get())->ref)
        {
            node.expr = new AddrOfNode(node.expr->location(), node.expr);
        }
    }
    else
    {
        if(TypeVisitor::type(c, node.target.get())->ref)
        {
            node.target = new DerefNode(node.target->location(), node.target);
        }
    }
}

void ExprLower::visit(BinaryNode &node)
{
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

void ExprLower::visit(ThisNode &node)
{
    rn = new DerefNode(node.location(), cn);
}

void ExprLower::visit(AddrOfNode &node)
{
    node.expr = ExprLower::lower(c, node.expr);
}

NodePtr ExprLower::lower(Context &c, NodePtr &node, const Type *type, Flags flags)
{
    ExprLower el(c, node, type, flags);
    node->accept(el);

    return el.result() ? el.result() : node;
}

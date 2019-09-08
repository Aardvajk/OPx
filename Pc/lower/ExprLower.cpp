#include "ExprLower.h"

#include "nodes/BlockNode.h"
#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/IncDecNode.h"

#include "visitors/TypeVisitor.h"

#include "syms/Sym.h"

#include "types/Type.h"
#include "types/TypeLookup.h"

#include "lower/FuncLower.h"

ExprLower::ExprLower(Context &c, NodePtr &cn, const Type *type, Flags flags) : c(c), cn(cn), type(type), flags(flags)
{
}

void ExprLower::visit(BlockNode &node)
{
    for(std::size_t i = 0; i < node.nodes.size(); ++i)
    {
        FuncLower fl(c);
        node.nodes[i]->accept(fl);
    }
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
                auto dn = new DerefNode(node.location(), cn);
                rn = dn;

                dn->setProperty("lower_generated", true);
            }
        }
        else if(type && type->ref)
        {
            auto an = new AddrOfNode(node.location(), cn);
            rn = an;

            an->setProperty("lower_generated", true);
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

    std::size_t off = t->args.size() > node.params.size() ? 1 : 0;

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprLower::lower(c, node.params[i], t->args[i + off], Flag::NoThisDeref);
    }

    if(!(flags & Flag::NoTopLevel))
    {
        if(t->returnType && t->returnType->ref && (!type || !type->ref))
        {
            auto dn = new DerefNode(node.location(), cn);
            rn = dn;

            dn->setProperty("lower_generated", true);
        }
    }
}

void ExprLower::visit(AssignNode &node)
{
    if(node.getProperty("constructor").value<bool>())
    {
        node.target = ExprLower::lower(c, node.target, nullptr, Flag::NoTopLevel);
        node.expr = ExprLower::lower(c, node.expr, nullptr, Flag::NoTopLevel);

        if(TypeVisitor::type(c, node.target.get())->ref && !TypeVisitor::type(c, node.expr.get())->ref)
        {
            auto an = new AddrOfNode(node.expr->location(), node.expr);
            node.expr = an;

            an->setProperty("lower_generated", true);
        }
    }
    else
    {
        node.target = ExprLower::lower(c, node.target, nullptr);
        node.expr = ExprLower::lower(c, node.expr, nullptr);
    }
}

void ExprLower::visit(BinaryNode &node)
{
    node.left = ExprLower::lower(c, node.left);
    node.right = ExprLower::lower(c, node.right);
}

void ExprLower::visit(ThisNode &node)
{
    if(!(flags & Flag::NoThisDeref))
    {
        if(!type || !type->ref)
        {
            auto dn = new DerefNode(node.location(), cn);
            rn = dn;

            dn->setProperty("lower_generated", true);
        }
    }
}

void ExprLower::visit(AddrOfNode &node)
{
    if(!node.getProperty("lower_generated").value<bool>())
    {
        node.expr = ExprLower::lower(c, node.expr);
    }
}

void ExprLower::visit(DerefNode &node)
{
    if(!node.getProperty("lower_generated").value<bool>())
    {
        node.expr = ExprLower::lower(c, node.expr);
    }
}

void ExprLower::visit(IncDecNode &node)
{
    node.target = ExprLower::lower(c, node.target);
}

NodePtr ExprLower::lower(Context &c, NodePtr &node, const Type *type, Flags flags)
{
    ExprLower el(c, node, type, flags);
    node->accept(el);

    return el.result() ? el.result() : node;
}

#include "ExprTransform.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/ThisNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "decorator/ExprDecorator.h"

#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

ExprTransform::ExprTransform(Context &c) : c(c)
{
}

void ExprTransform::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprTransform::transform(c, node.parent);
    }
    else
    {
        if(auto s = node.findProperty("sym"))
        {
            auto sym = s.to<Sym*>();

            if(sym->findProperty("member").value<bool>() || sym->findProperty("method").value<bool>())
            {
                auto n = new ThisNode(node.location());
                node.parent = n;

                auto t = Type::makePrimary(sym->parent());

                t.ref = true;
                t.constant = c.tree.current()->container()->property<Type*>("type")->constMethod;

                n->setProperty("type", c.types.insert(t));
            }
        }
    }
}

void ExprTransform::visit(CallNode &node)
{
    node.target = ExprTransform::transform(c, node.target);

    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprTransform::transform(c, node.params[i]);
    }

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(type->method)
    {
        node.params.insert(node.params.begin(), Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get()));
    }
}

void ExprTransform::visit(ConstructNode &node)
{
    for(std::size_t i = 0; i < node.params.size(); ++i)
    {
        node.params[i] = ExprTransform::transform(c, node.params[i]);
    }

    if(!node.type->primitive())
    {
        auto t = Type::makeFunction(c.types.nullType(), { c.types.insert(Type::makePrimary(false, true, node.type->sym)) });
        for(auto &p: node.params)
        {
            t.args.push_back(TypeVisitor::assertType(c, p.get()));
        }

        NodePtr name(new IdNode(node.location(), { }, node.type->sym->name()));
        node.target = new IdNode(node.location(), name, "new");

        node.target = ExprDecorator::decorate(c, node.target, c.types.insert(t));

        auto info = c.tree.current()->container()->property<FuncInfo*>("info");
        auto temp = pcx::str("temp", info->labels++);

        info->temps.push_back(std::make_pair(temp, node.type));
        node.setProperty("temp", temp);
    }
}

void ExprTransform::visit(AddrOfNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(DerefNode &node)
{
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(AssignNode &node)
{
    node.target = ExprTransform::transform(c, node.target);
    node.expr = ExprTransform::transform(c, node.expr);
}

void ExprTransform::visit(BinaryNode &node)
{
    node.left = ExprTransform::transform(c, node.left);
    node.right = ExprTransform::transform(c, node.right);
}

NodePtr ExprTransform::transform(Context &c, NodePtr &node)
{
    ExprTransform et(c);
    node->accept(et);

    return et.result() ? et.result() : node;
}

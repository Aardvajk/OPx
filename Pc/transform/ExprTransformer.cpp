#include "ExprTransformer.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/AssignNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"

#include "decorator/ExprDecorator.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "transform/ThisCallTransformer.h"

ExprTransformer::ExprTransformer(Context &c) : c(c)
{
}

void ExprTransformer::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprTransformer::transform(c, node.parent);
    }
    else
    {
        if(auto s = node.getProperty("sym"))
        {
            auto sym = s.to<const Sym*>();

            if(sym->getProperty("member").value<bool>() || sym->getProperty("method").value<bool>())
            {
                auto n = new DerefNode(node.location());
                n->expr = new ThisNode(node.location());

                node.parent = n;
            }
        }
    }
}

void ExprTransformer::visit(AssignNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    node.expr = ExprTransformer::transform(c, node.expr);

    if(!TypeVisitor::type(c, node.target.get())->primitive())
    {
        auto cn = new CallNode(node.location(), new IdNode(node.location(), { }, "operator="));
        rn = cn;

        cn->params.push_back(new AddrOfNode(node.target->location(), node.target));
        cn->params.push_back(node.expr);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
}

void ExprTransformer::visit(CallNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    ExprTransformer::transform(c, node.params);

    if(TypeVisitor::type(c, node.target.get())->method)
    {
        ThisCallTransformer tt(c);
        node.target->accept(tt);

        if(!tt.result())
        {
            throw Error(node.location(), "cannot call method without object - ", NameVisitors::prettyName(node.target.get()));
        }

        node.params.insert(node.params.begin(), tt.result());
    }
}

void ExprTransformer::visit(AddrOfNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}

void ExprTransformer::visit(ThisNode &node)
{
    auto f = c.tree.current()->container();

    if(f->parent()->type() == Sym::Type::Class)
    {
        node.setProperty("sym", c.tree.current()->container()->child("this"));
    }
    else
    {
        throw Error(node.location(), "this cannot be used in a static function");
    }
}

void ExprTransformer::visit(DerefNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);
}

NodePtr ExprTransformer::transform(Context &c, NodePtr &node)
{
    ExprTransformer et(c);
    node->accept(et);

    return et.result() ? et.result() : node;
}

void ExprTransformer::transform(Context &c, NodeList &nodes)
{
    for(std::size_t i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = ExprTransformer::transform(c, nodes[i]);
    }
}

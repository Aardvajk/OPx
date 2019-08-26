#include "ExprTransformer.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/AssignNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/ThisNode.h"
#include "nodes/DerefNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/PrimitiveCastNode.h"
#include "nodes/LogicalNode.h"

#include "decorator/ExprDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

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
            auto sym = s.to<Sym*>();

            if(sym->getProperty("member").value<bool>() || sym->getProperty("method").value<bool>())
            {
                auto n = new DerefNode(node.location());
                n->expr = new ThisNode(node.location());

                node.parent = n;
            }
        }
    }
}

void ExprTransformer::visit(StringLiteralNode &node)
{
    auto name = pcx::str("#global.", c.sources.path(c.scanner.sourceId()), c.globals.size());

    c.globals[name] = &node;
    node.setProperty("global", name);
}

void ExprTransformer::visit(AssignNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    node.expr = ExprTransformer::transform(c, node.expr);

    auto tt = TypeVisitor::type(c, node.target.get());

    if(!tt->primitive() && !tt->ref)
    {
        auto cn = new CallNode(node.location(), new IdNode(node.location(), { }, "operator="));
        rn = cn;

        cn->params.push_back(new AddrOfNode(node.target->location(), node.target));
        cn->params.push_back(node.expr);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
    else
    {
        auto et = TypeVisitor::type(c, node.expr.get());
        if(!TypeCompare::compatible(tt, et))
        {
            throw Error(node.location(), "different primitive types to operator= - ", tt->text(), " and ", et->text());
        }
    }
}

void ExprTransformer::visit(CallNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    ExprTransformer::transform(c, node.params);

    auto t = TypeVisitor::type(c, node.target.get());

    if(t->method)
    {
        ThisCallTransformer tt(c);
        node.target->accept(tt);

        auto tn = tt.result();

        if(!tn)
        {
            throw Error(node.location(), "cannot call method without object - ", NameVisitors::prettyName(node.target.get()));
        }

        ExprDecorator::decorate(c, nullptr, *tn);

        node.params.insert(node.params.begin(), tn);
    }
    else if(!t->function())
    {
        if(t->primitive())
        {
            if(node.params.size() > 1)
            {
                throw Error(node.location(), "invalid cast syntax - ", NameVisitors::prettyName(node.target.get()));
            }

            if(node.params.empty())
            {
                rn = new PrimitiveCastNode(node.location(), t);
            }
            else
            {
                auto p = node.params.front();

                if(TypeVisitor::type(c, p.get())->primitive())
                {
                    rn = new PrimitiveCastNode(node.location(), t, p);
                }
                else
                {
                    throw Error("internal error - cast from non-primitive to primitive not supported");
                }
            }
        }
        else
        {
            auto temp = pcx::str("#temp", c.labels++);
            c.temps[c.tree.current()->container()].push_back(std::make_pair(temp, t));

            node.setProperty("temp", temp);
        }
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

void ExprTransformer::visit(UnaryNode &node)
{
    node.expr = ExprTransformer::transform(c, node.expr);

    auto t = TypeVisitor::type(c, node.expr.get());

    if(!t->primitive())
    {
        auto cn = new CallNode(node.location(), new IdNode(node.location(), { }, pcx::str("operator", Operators::toString(node.op))));
        rn = cn;

        cn->params.push_back(node.expr);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
}

void ExprTransformer::visit(BinaryNode &node)
{
    node.left = ExprTransformer::transform(c, node.left);
    node.right = ExprTransformer::transform(c, node.right);

    auto lt = TypeVisitor::type(c, node.left.get());
    auto rt = TypeVisitor::type(c, node.right.get());

    if(!lt->primitive() || !rt->primitive())
    {
        auto cn = new CallNode(node.location(), new IdNode(node.location(), { }, pcx::str("operator", Operators::toString(node.op))));
        rn = cn;

        cn->params.push_back(node.left);
        cn->params.push_back(node.right);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
    else
    {
        if(!TypeCompare::compatible(lt, rt))
        {
            throw Error(node.location(), "different primitive types to operator", Operators::toString(node.op), " - ", lt->text(), " and ", rt->text());
        }
    }
}

void ExprTransformer::visit(SubscriptNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    node.expr = ExprTransformer::transform(c, node.expr);

    auto dn = new DerefNode(node.location());
    rn = dn;

    dn->expr = new BinaryNode(node.location(), Operators::Type::Add, node.target, node.expr);
}

void ExprTransformer::visit(LogicalNode &node)
{
    node.left = ExprTransformer::transform(c, node.left);
    node.right = ExprTransformer::transform(c, node.right);
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

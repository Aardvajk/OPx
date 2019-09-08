#include "ExprTransformer.h"

#include "application/Context.h"

#include "nodes/BlockNode.h"
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
#include "nodes/IncDecNode.h"
#include "nodes/OpEqNode.h"

#include "decorator/ExprDecorator.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "visitors/TypeVisitor.h"
#include "visitors/NameVisitors.h"

#include "transform/ThisCallTransformer.h"
#include "transform/FuncTransformer.h"

namespace
{

void handleLiteral(Context &c, const Type *expectedType, Node &node)
{
    if(expectedType && expectedType->ref)
    {
        auto name = pcx::str("#temp_literal", c.labels++);

        node.setProperty("temp_literal", name);
        c.temps[c.tree.current()->container()].push_back(std::make_pair(name, TypeVisitor::type(c, &node)));
    }
}

}

ExprTransformer::ExprTransformer(Context &c, const Type *expectedType) : c(c), expectedType(expectedType)
{
}

void ExprTransformer::visit(BlockNode &node)
{
    FuncTransformer ft(c);
    node.accept(ft);
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
                node.parent = new ThisNode(node.location());
            }
        }
    }
}

void ExprTransformer::visit(CharLiteralNode &node)
{
    handleLiteral(c, expectedType, node);
}

void ExprTransformer::visit(IntLiteralNode &node)
{
    handleLiteral(c, expectedType, node);
}

void ExprTransformer::visit(BoolLiteralNode &node)
{
    handleLiteral(c, expectedType, node);
}

void ExprTransformer::visit(SizeLiteralNode &node)
{
    handleLiteral(c, expectedType, node);
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

    if(!tt->primitive() && (node.getProperty("constructor").value<bool>() ? !tt->ref : true))
    {
        NodePtr pn(new IdNode(node.location(), { }, tt->sym->name()));

        auto cn = new CallNode(node.location(), new IdNode(node.location(), pn, "operator="));
        rn = cn;

        cn->params.push_back(node.target);
        cn->params.push_back(node.expr);

        ExprDecorator::decorate(c, nullptr, *cn);
    }
    else
    {
        if(!node.getProperty("constructor").value<bool>())
        {
            c.assertMutable(node.target->location(), tt);

            if(tt->ptr || tt->ref)
            {
                if(TypeVisitor::type(c, node.expr.get())->constant)
                {
                    throw Error(node.expr->location(), "cannot assign const to mutable - ", NameVisitors::prettyName(node.expr.get()));
                }
            }
        }

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

    auto t = TypeVisitor::type(c, node.target.get());

    if(t->primitive() && !t->function())
    {
        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprTransformer::transform(c, node.params[i]);
        }
    }
    else
    {
        auto ft = t;
        if(!ft->function())
        {
            ft = node.target->property<Sym*>("newmethod")->property<Type*>("type");
        }

        std::size_t off = ft->args.size() > node.params.size() ? 1 : 0;

        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprTransformer::transform(c, node.params[i], ft->args[i + off]);
        }
    }

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
            auto temp = pcx::str("#temp_", t->text(), c.labels++);
            c.temps[c.tree.current()->container()].push_back(std::make_pair(temp, t));

            node.setProperty("temp", temp);
        }
    }

    if(t->function())
    {
        auto r = t->returnType;
        if(!r->primitive() && !r->ref)
        {
            auto temp = pcx::str("#tempreturn_", r->text(), c.labels++);
            c.temps[c.tree.current()->container()].push_back(std::make_pair(temp, r));

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
        if(!lt->ptr && !rt->ptr && !TypeCompare::compatible(lt, rt))
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

void ExprTransformer::visit(IncDecNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);

    auto t = TypeVisitor::type(c, node.target.get());

    if(t->primitive())
    {
        c.assertMutable(node.target->location(), t);
    }
    else
    {
        NodePtr fn(new IdNode(node.location(), { }, node.op == Operators::Type::PreInc || node.op == Operators::Type::PostInc ? "operator++" : "operator--"));

        auto cn = new CallNode(node.location(), fn);
        rn = cn;

        auto pn = IdNode::create(node.location(), { "std", node.op == Operators::Type::PreInc || node.op == Operators::Type::PreDec ? "prefix" : "postfix" });
        pn->setProperty("newmethod", c.assertChainedSym(node.location(), c.tree.root(), { "std", "prefix", "new" }));

        cn->params.push_back(new CallNode(node.location(), pn));
        cn->params.push_back(node.target);

        ExprDecorator::decorate(c, nullptr, *cn);
        rn = ExprTransformer::transform(c, rn);
    }
}

void ExprTransformer::visit(OpEqNode &node)
{
    node.target = ExprTransformer::transform(c, node.target);
    node.expr = ExprTransformer::transform(c, node.expr);

    if(TypeVisitor::type(c, node.target.get())->primitive())
    {
        auto an = new AssignNode(node.location(), node.target);
        rn = an;

        an->expr = new BinaryNode(node.location(), Operators::opFromOpEq(node.op), node.target, node.expr);
        ExprTransformer::transform(c, rn);
    }
    else
    {
        NodePtr fn(new IdNode(node.location(), { }, pcx::str("operator", Operators::toString(node.op))));

        auto cn = new CallNode(node.location(), fn);
        rn = cn;

        cn->params.push_back(node.target);
        cn->params.push_back(node.expr);

        ExprDecorator::decorate(c, nullptr, *rn);
        rn = ExprTransformer::transform(c, rn);
    }
}

NodePtr ExprTransformer::transform(Context &c, NodePtr &node, const Type *expectedType)
{
    ExprTransformer et(c, expectedType);
    node->accept(et);

    return et.result() ? et.result() : node;
}

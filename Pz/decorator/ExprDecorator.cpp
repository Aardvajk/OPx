#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/BinaryNode.h"

#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include <unordered_set>

namespace
{

std::vector<Sym*> pruneResult(const std::vector<Sym*> &sv, const Type *expectedType)
{
    std::vector<Sym*> r;

    for(auto s: sv)
    {
        auto type = s->property<Type*>("type");

        if(type->constMethod == expectedType->constMethod)
        {
            r.push_back(s);
        }
    }

    return r;
}

}

ExprDecorator::ExprDecorator(Context &c, Type *expectedType, Flags flags) : c(c), expectedType(expectedType), flags(flags)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.parent)
    {
        node.parent = ExprDecorator::decorate(c, node.parent);

        auto t = TypeVisitor::queryType(c, node.parent.get());
        if(t && t->ptr)
        {
            throw Error(node.location(), "cannot access via pointer - ", node.description());
        }
    }

    std::unordered_set<Sym*> search;

    if(expectedType)
    {
        for(auto &a: expectedType->args)
        {
            if(a->sym && a->sym->parent() != c.tree.root())
            {
                search.insert(a->sym->parent());
            }
        }
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

    for(auto s: search)
    {
        SymFinder::find(c, SymFinder::Type::Local, s, &node, sv);
    }

    if(expectedType && expectedType->function())
    {
        std::vector<Sym*> r;

        for(auto s: sv)
        {
            if(s->type() == Sym::Type::Func)
            {
                auto type = s->property<Type*>("type");

                if(TypeCompare(c).compatibleArgs(type, expectedType) && (!expectedType->constMethod || type->constMethod))
                {
                    r.push_back(s);
                }
            }
            else
            {
                r.push_back(s);
            }
        }

        sv = r;
    }

    if(sv.size() > 1 && expectedType)
    {
        sv = pruneResult(sv, expectedType);
    }

    if(sv.empty())
    {
        throw Error(node.location(), "not found - ", node.description(), (expectedType ? expectedType->text() : ""));
    }
    else if(sv.size() > 1)
    {
        throw Error(node.location(), "ambigous - ", node.description());
    }

    node.setProperty("sym", sv.front());
}

void ExprDecorator::visit(StringLiteralNode &node)
{
    auto name = pcx::str("#global.", c.globals.size());

    c.globals[name] = &node;
    node.setProperty("global", name);
}

void ExprDecorator::visit(CallNode &node)
{
    auto t = Type::makeFunction(c.types.nullType());

    if(!flags[Flag::SkipParams])
    {
        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprDecorator::decorate(c, node.params[i]);
        }
    }

    for(auto &p: node.params)
    {
        t.args.push_back(TypeVisitor::assertType(c, p.get()));
    }

    node.target = ExprDecorator::decorate(c, node.target, &t);

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(type->method)
    {
        auto n = Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get());
        if(!n)
        {
            throw Error(node.target->location(), "cannot call method without object - ", node.target->description());
        }

        if(TypeVisitor::assertType(c, n.get())->constant)
        {
            t.constMethod = true;

            node.target = ExprDecorator::decorate(c, node.target, &t);
        }
    }

    if(auto dt = Visitor::query<QueryVisitors::DirectType, Type*>(node.target.get()))
    {
        rn = new ConstructNode(node.location(), dt, node.params);
    }
    else
    {
        if(!type->returnType)
        {
            NodePtr id(new IdNode(node.location(), { }, "operator()"));

            auto cn = new CallNode(node.location(), id);
            rn = cn;

            cn->params.push_back(node.target);
            for(auto &p: node.params)
            {
                cn->params.push_back(p);
            }

            rn = ExprDecorator::decorate(c, rn, nullptr, Flag::SkipParams);
        }
    }
}

void ExprDecorator::visit(AddrOfNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
    node.setProperty("type", c.types.insert(TypeVisitor::assertType(c, node.expr.get())->addPointer()));
}

void ExprDecorator::visit(DerefNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    if(!TypeVisitor::assertType(c, node.expr.get())->ptr)
    {
        throw Error(node.location(), "cannot deref a non-pointer - ", node.description());
    }

    node.setProperty("type", c.types.insert(TypeVisitor::assertType(c, node.expr.get())->removePointer()));
}

void ExprDecorator::visit(ThisNode &node)
{
    auto s = c.tree.current()->container();

    if(s->parent()->type() != Sym::Type::Class)
    {
        throw Error(node.location(), "this outside method");
    }

    auto t = Type::makePrimary(s->parent());

    t.ref = true;
    t.constant = s->property<Type*>("type")->constMethod;

    node.setProperty("type", c.types.insert(t));
}

void ExprDecorator::visit(AssignNode &node)
{
    node.target = ExprDecorator::decorate(c, node.target);

    if(TypeVisitor::assertType(c, node.target.get())->constant)
    {
        throw Error(node.target->location(), "cannot assign to const - ", node.target->description());
    }

    node.expr = ExprDecorator::decorate(c, node.expr, TypeVisitor::assertType(c, node.target.get()));
}

void ExprDecorator::visit(BinaryNode &node)
{
    node.left = ExprDecorator::decorate(c, node.left);
    node.right = ExprDecorator::decorate(c, node.right);

    auto lt = TypeVisitor::assertType(c, node.left.get());
    auto rt = TypeVisitor::assertType(c, node.right.get());

    if(!lt->primitive() || !rt->primitive())
    {
        NodePtr id(new IdNode(node.location(), { }, pcx::str("operator", node.token.text())));

        auto cn = new CallNode(node.location(), id);
        rn = cn;

        cn->params.push_back(node.left);
        cn->params.push_back(node.right);

        rn = ExprDecorator::decorate(c, rn, nullptr, Flag::SkipParams);
    }
}

NodePtr ExprDecorator::decorate(Context &c, NodePtr &node, Type *expectedType, Flags flags)
{
    ExprDecorator ed(c, expectedType, flags);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}

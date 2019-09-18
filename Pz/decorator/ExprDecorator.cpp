#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"

#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

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
    }

    std::vector<Sym*> sv;
    SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);

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

NodePtr ExprDecorator::decorate(Context &c, NodePtr &node, Type *expectedType, Flags flags)
{
    ExprDecorator ed(c, expectedType, flags);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}

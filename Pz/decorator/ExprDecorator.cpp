#include "ExprDecorator.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ConstructNode.h"

#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

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

                if(TypeCompare(c).compatibleArgs(type, expectedType) && type->constMethod == expectedType->constMethod)
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

    if(sv.empty())
    {
        throw Error(node.location(), "not found - ", node.description(), (expectedType ? expectedType->text() : ""));
    }
    else if(sv.size() > 1)
    {
        throw Error(node.location(), "ambigous - ", node.description());
    }

    node.setProperty("sym", sv.front());

    if(auto t = sv.front()->findProperty("type"))
    {
        node.setProperty("type", t.to<Type*>());
    }
}

void ExprDecorator::visit(IntLiteralNode &node)
{
    node.setProperty("type", c.types.intType());
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
    node.setProperty("type", TypeVisitor::assertType(c, node.target.get()));

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(type->method)
    {
        auto n = Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get());
        if(!n)
        {
            throw Error(node.target->location(), "cannot call method without parent - ", node.target->description());
        }

        if(TypeVisitor::assertType(c, n.get())->constant)
        {
            t.constMethod = true;

            node.target = ExprDecorator::decorate(c, node.target, &t);
            node.setProperty("type", TypeVisitor::assertType(c, node.target.get()));
        }
    }

    if(auto dt = Visitor::query<QueryVisitors::DirectType, Type*>(node.target.get()))
    {
        rn = new ConstructNode(node.location(), dt, node.params);
        rn->setProperty("type", dt);
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

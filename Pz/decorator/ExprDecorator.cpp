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

    if(sv.size() > 1)
    {
        if(expectedType && expectedType->function())
        {
            std::vector<Sym*> r;

            for(auto s: sv)
            {
                auto type = s->property<Type*>("type");

                if(TypeCompare(c).compatibleArgs(type, expectedType))
                {
                    r.push_back(s);
                }
            }

            sv = r;
        }
    }

    if(sv.empty())
    {
        throw Error(node.location(), "not found - ", node.description());
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

    if(!(flags & Flag::SkipParams))
    {
        for(std::size_t i = 0; i < node.params.size(); ++i)
        {
            node.params[i] = ExprDecorator::decorate(c, node.params[i]);
            t.args.push_back(TypeVisitor::assertType(c, node.params[i].get()));
        }
    }

    node.target = ExprDecorator::decorate(c, node.target, &t);
    node.setProperty("type", TypeVisitor::assertType(c, node.target.get()));

    if(auto type = Visitor::query<QueryVisitors::DirectType, Type*>(node.target.get()))
    {
        rn = new ConstructNode(node.location(), type, node.params);
        rn->setProperty("type", type);
    }
    else if(!TypeVisitor::assertType(c, node.target.get())->returnType)
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

NodePtr ExprDecorator::decorate(Context &c, NodePtr &node, Type *expectedType, Flags flags)
{
    ExprDecorator ed(c, expectedType, flags);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}

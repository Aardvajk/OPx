#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/AssignNode.h"
#include "nodes/DerefNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/SubscriptNode.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/CommonDecorator.h"

ExprDecorator::ExprDecorator(Context &c, const Type *expectedType) : c(c), expectedType(expectedType)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(expectedType && expectedType->function())
    {
        node.setProperty("sym", CommonDecorator::searchCallableByType(c, node, expectedType));
    }
    else
    {
        std::vector<Sym*> sv;
        SymFinder::find(SymFinder::Type::Global, c.tree.current(), &node, sv);

        if(sv.empty())
        {
            throw Error(node.location(), "not found - ", NameVisitors::prettyName(&node));
        }

        if(sv.size() > 1)
        {
            throw Error(node.location(), "ambigious reference - ", NameVisitors::prettyName(&node));
        }

        node.setProperty("sym", sv.front());
    }
}

void ExprDecorator::visit(CallNode &node)
{
    for(auto &p: node.params)
    {
        p->accept(*this);
    }

    auto t = Type::makeFunction(0, c.types.nullType());
    for(auto &p: node.params)
    {
        auto pt = TypeVisitor::type(c, p.get());
        if(!pt)
        {
            throw Error(p->location(), "internal type lookup failed - ", NameVisitors::prettyName(p.get()));
        }

        t.args.push_back(pt);
    }

    ExprDecorator ed(c, c.types.insert(t));
    node.target->accept(ed);
}

void ExprDecorator::visit(AddrOfNode &node)
{
    node.expr->accept(*this);
}

void ExprDecorator::visit(AssignNode &node)
{
    node.target->accept(*this);
    node.expr->accept(*this);

    auto type = TypeVisitor::type(c, node.target.get());

    if(type->primitive())
    {
        if(!TypeCompare::exact(type, TypeVisitor::type(c, node.expr.get())))
        {
            throw Error(node.expr->location(), type->text(), " expected - ", NameVisitors::prettyName(node.expr.get()));
        }
    }
}

void ExprDecorator::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

void ExprDecorator::visit(BinaryNode &node)
{
    node.left->accept(*this);
    node.right->accept(*this);
}

void ExprDecorator::visit(SubscriptNode &node)
{
    node.target->accept(*this);
    node.expr->accept(*this);
}

void ExprDecorator::decorate(Context &c, const Type *expectedType, Node &node)
{
    ExprDecorator ed(c, expectedType);
    node.accept(ed);
}

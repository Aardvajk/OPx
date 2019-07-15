#include "ExprDecorator.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/IdNode.h"
#include "nodes/CallNode.h"

#include "visitors/SymFinder.h"
#include "visitors/NameVisitors.h"
#include "visitors/TypeVisitor.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

namespace
{

Sym *searchFunction(Location location, const std::vector<Sym*> &sv, const Type *expectedType)
{
    for(auto s: sv)
    {
        if(s->type() != Sym::Type::Func)
        {
            throw Error(location, "function expected - ", s->fullname());
        }

        if(TypeCompare::args(expectedType, s->property("type").to<const Type*>()))
        {
            return s;
        }
    }

    return nullptr;
}

}

ExprDecorator::ExprDecorator(Context &c, const Type *expectedType) : c(c), expectedType(expectedType)
{
}

void ExprDecorator::visit(IdNode &node)
{
    auto sv = SymFinder::find(SymFinder::Type::Global, c.tree.current(), &node);
    if(expectedType)
    {
        auto s = searchFunction(node.location(), sv, expectedType);
        if(!s)
        {
            throw Error(node.location(), "no function matched - ", NameVisitors::prettyName(&node));
        }

        node.setProperty("sym", s);
    }
    else
    {
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
        p.accept(*this);
    }

    Type t;
    for(auto &p: node.params)
    {
        t.args.push_back(TypeVisitor::type(c, &p));
    }

    ExprDecorator ed(c, c.types.insert(t));
    node.target->accept(ed);
}

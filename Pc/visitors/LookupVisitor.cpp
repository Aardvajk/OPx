#include "LookupVisitor.h"

#include "framework/Error.h"

#include "application/Context.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"
#include "nodes/CallNode.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"

namespace
{

void lookup(Context &c, Node &node)
{
    SymFinder sf(SymFinder::Policy::Full, c.tree.root(), c.tree.current());
    node.accept(sf);

    node.setProperty("syms", sf.result());
}

}

LookupVisitor::LookupVisitor(Context &c) : c(c)
{
}

void LookupVisitor::visit(GlobalNode &node)
{
    lookup(c, node);
}

void LookupVisitor::visit(IdNode &node)
{
    lookup(c, node);
}

void LookupVisitor::visit(DotNode &node)
{
    lookup(c, node);
}

void LookupVisitor::visit(CallNode &node)
{
    Type searchType;
    for(auto &a: node.args)
    {
        TypeVisitor tv(c);
        a.accept(tv);

        if(!tv.result())
        {
            throw Error(a.location(), "type not detectable - ", NameVisitors::prettyName(&a));
        }

        searchType.args.push_back(tv.result()->clone());
    }

    auto sym = c.matchFunction(node.target->location(), node.target->property("syms").value<std::vector<SymResult> >(), &searchType);

    node.setProperty("sym", sym);
}

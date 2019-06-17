#include "LookupVisitor.h"

#include "application/Context.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

#include "nodes/GlobalNode.h"
#include "nodes/IdNode.h"
#include "nodes/DotNode.h"

#include <iostream>

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

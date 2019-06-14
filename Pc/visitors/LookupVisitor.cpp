#include "LookupVisitor.h"

#include "application/Context.h"

#include "visitors/NameVisitors.h"
#include "visitors/SymFinder.h"

#include "nodes/IdNode.h"

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

void LookupVisitor::visit(IdNode &node)
{
    lookup(c, node);
}

#include "TakesAddrVisitor.h"

#include "nodes/Node.h"

TakesAddrVisitor::TakesAddrVisitor() : r(false)
{
}

void TakesAddrVisitor::visit(AddrOfNode &node)
{
    r = true;
}

bool TakesAddrVisitor::examine(Node &node)
{
    TakesAddrVisitor tv;
    node.accept(tv);

    return tv.result();
}

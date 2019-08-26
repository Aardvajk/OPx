#include "SymScopeVisitor.h"

#include "nodes/IdNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

SymScopeVisitor::SymScopeVisitor(Sym *curr) : curr(curr)
{
}

void SymScopeVisitor::visit(AddrOfNode &node)
{
    node.expr->accept(*this);
}

void SymScopeVisitor::visit(ThisNode &node)
{
    curr = curr->container()->parent();
}

void SymScopeVisitor::visit(DerefNode &node)
{
    node.expr->accept(*this);
}

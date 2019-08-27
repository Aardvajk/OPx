#include "SymScopeVisitor.h"

#include "nodes/CallNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

SymScopeVisitor::SymScopeVisitor(Context &c, Sym *curr) : c(c), curr(curr)
{
}

void SymScopeVisitor::visit(CallNode &node)
{
    auto t = TypeVisitor::type(c, node.target.get());
    if(t->returnType && t->returnType->sym)
    {
        curr = t->returnType->sym;
    }
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

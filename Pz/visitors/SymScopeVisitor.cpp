#include "SymScopeVisitor.h"

#include "nodes/CallNode.h"

#include "syms/Sym.h"

#include "types/Type.h"

#include "visitors/TypeVisitor.h"

SymScopeVisitor::SymScopeVisitor(Context &c, Sym *curr) : c(c), curr(curr)
{
}

void SymScopeVisitor::visit(CallNode &node)
{
    auto t = TypeVisitor::assertType(c, node.target.get());
    if(t->returnType && t->returnType->sym)
    {
        curr = t->returnType->sym;
    }
}

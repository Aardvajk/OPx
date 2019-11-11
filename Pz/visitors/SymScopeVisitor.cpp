#include "SymScopeVisitor.h"

#include "syms/Sym.h"

SymScopeVisitor::SymScopeVisitor(Context &c, Sym *curr) : c(c), curr(curr)
{
}

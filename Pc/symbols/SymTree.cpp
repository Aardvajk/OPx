#include "SymTree.h"

SymTree::SymTree() : r(new Sym(Sym::Type::Global, { }, "[global]")), cs(r.get())
{
}

SymGuard SymTree::open(Sym *sym)
{
    SymGuard g(this, cs);
    cs = sym;

    return g;
}

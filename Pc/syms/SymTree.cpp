#include "SymTree.h"

SymTree::SymTree() : rs(new Sym(Sym::Type::Namespace, { }, { })), cs(rs.get())
{
}

SymGuard SymTree::open(Sym *sym)
{
    SymGuard g(this, cs);
    cs = sym;

    return g;
}

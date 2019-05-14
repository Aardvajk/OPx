#include "SymTree.h"

SymTree::SymTree() : r(new Sym(Sym::Type::Namespace, Sym::Attr::Public, { }, { })), cs(r.get())
{
}

SymGuard SymTree::open(Sym *sym)
{
    SymGuard g(this, cs);
    cs = sym;

    return g;
}

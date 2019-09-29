#include "SymTree.h"

SymTree::SymTree() : rs(new Sym(Sym::Type::Namespace, { }, Access::Public, { })), cs(rs.get())
{
}

SymGuard SymTree::open(Sym *sym)
{
    SymGuard g(this, cs);
    cs = sym;

    return g;
}

std::size_t SymTree::scopeDepth()
{
    auto s = current();
    std::size_t n = 0;

    while(s && s != current()->container())
    {
        s = s->parent();
        ++n;
    }

    return n;
}

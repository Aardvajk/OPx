#ifndef SYMTREE_H
#define SYMTREE_H

#include "symbols/Sym.h"
#include "symbols/SymGuard.h"

#include <pcx/scoped_ptr.h>

class SymTree
{
public:
    SymTree();

    SymGuard open(Sym *sym);

    Sym *root(){ return r.get(); }
    Sym *current(){ return cs; }

    const Sym *current() const { return cs; }

private:
    friend class SymGuard;

    pcx::scoped_ptr<Sym> r;
    Sym *cs;
};

#endif // SYMTREE_H

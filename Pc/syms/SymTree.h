#ifndef SYMTREE_H
#define SYMTREE_H

#include "syms/Sym.h"
#include "syms/SymGuard.h"

#include <pcx/scoped_ptr.h>

class SymTree
{
public:
    SymTree();

    SymGuard open(Sym *sym);

    Sym *root(){ return rs.get(); }
    Sym *current(){ return cs; }

private:
    friend class SymGuard;

    pcx::scoped_ptr<Sym> rs;
    Sym *cs;
};

#endif // SYMTREE_H

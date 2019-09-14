#include "SymGuard.h"

#include "syms/SymTree.h"

SymGuard::~SymGuard()
{
    if(t)
    {
        t->cs = s;
    }
}

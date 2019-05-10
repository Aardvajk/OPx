#include "SymGuard.h"

#include "symbols/SymTree.h"

SymGuard::~SymGuard()
{
    if(t)
    {
        t->cs = s;
    }
}

#ifndef SYMGUARD_H
#define SYMGUARD_H

#include <pcx/non_copyable.h>

class Sym;
class SymTree;

class SymGuard : public pcx::non_copyable
{
public:
    SymGuard(SymTree *t, Sym *s) : t(t), s(s) { }
    SymGuard(SymGuard &&g) : t(g.t), s(g.s) { g.t = nullptr; }
    ~SymGuard();

private:
    SymTree *t;
    Sym *s;
};

#endif // SYMGUARD_H

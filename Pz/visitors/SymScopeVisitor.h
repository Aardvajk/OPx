#ifndef SYMSCOPEVISITOR_H
#define SYMSCOPEVISITOR_H

#include "visitors/Visitor.h"

class Context;
class Sym;

class SymScopeVisitor : public Visitor
{
public:
    SymScopeVisitor(Context &c, Sym *curr);

    Sym *result(){ return curr; }

private:
    Context &c;
    Sym *curr;
};

#endif // SYMSCOPEVISITOR_H

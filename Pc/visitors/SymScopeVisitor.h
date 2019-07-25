#ifndef SYMSCOPEVISITOR_H
#define SYMSCOPEVISITOR_H

#include "visitors/Visitor.h"

class Sym;

class SymScopeVisitor : public Visitor
{
public:
    explicit SymScopeVisitor(Sym *curr);

    Sym *result(){ return curr; }

    virtual void visit(AddrOfNode &node) override;
    virtual void visit(ThisNode &node) override;
    virtual void visit(DerefNode &node) override;

private:
    Sym *curr;
};

#endif // SYMSCOPEVISITOR_H

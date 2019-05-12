#ifndef SYMFINDER_H
#define SYMFINDER_H

#include "visitors/Visitor.h"

#include <vector>

class Sym;

class SymFinder : public Visitor
{
public:
    SymFinder(Sym *start, Sym *limit);

    std::vector<Sym*> result() const { return v; }

    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    Sym *start;
    Sym *limit;
    std::vector<Sym*> scopes;
    std::vector<Sym*> v;
};

#endif // SYMFINDER_H

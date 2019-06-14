#ifndef SYMFINDER_H
#define SYMFINDER_H

#include "visitors/Visitor.h"

#include "symbols/SymResult.h"

#include <utility>
#include <vector>

class Sym;

class SymFinder : public Visitor
{
public:
    enum class Policy { Full, Limited };

    SymFinder(Policy policy, Sym *root, Sym *start);

    std::vector<SymResult> result() const { return v; }

    virtual void visit(GlobalNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    Policy policy;

    Sym *root;
    Sym *start;

    std::vector<SymResult> scopes;
    std::vector<SymResult> v;

    std::vector<bool> access;
};

#endif // SYMFINDER_H

#ifndef SYMFINDER_H
#define SYMFINDER_H

#include "visitors/Visitor.h"

#include <vector>

class Sym;
class Node;

class SymFinder : public Visitor
{
public:
    explicit SymFinder(Sym *curr);

    std::vector<Sym*> result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

    static std::vector<Sym*> find(Sym *curr, Node *node);

private:
    std::vector<Sym*> curr;
    std::vector<Sym*> r;
};

#endif // SYMFINDER_H

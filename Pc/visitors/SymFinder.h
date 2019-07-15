#ifndef SYMFINDER_H
#define SYMFINDER_H

#include "visitors/Visitor.h"

#include <vector>

class Sym;
class Node;

class SymFinder : public Visitor
{
public:
    enum class Type
    {
        Global,
        Local
    };

    SymFinder(Type type, Sym *curr);

    std::vector<Sym*> result() const { return r; }

    virtual void visit(IdNode &node) override;

    static std::vector<Sym*> find(Type type, Sym *curr, Node *node);

private:
    Type type;

    std::vector<Sym*> curr;
    std::vector<Sym*> r;
};

#endif // SYMFINDER_H

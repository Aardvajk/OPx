#ifndef SYMFINDER_H
#define SYMFINDER_H

#include "visitors/Visitor.h"

#include <vector>

class Context;
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

    SymFinder(Context &c, Type type, Sym *curr, std::vector<Sym*> &result);

    virtual void visit(IdNode &node) override;

    static void find(Context &c, Type type, Sym *curr, Node *node, std::vector<Sym*> &result);

private:
    Context &c;
    Type type;

    Sym *curr;
    std::vector<Sym*> &r;
};

#endif // SYMFINDER_H

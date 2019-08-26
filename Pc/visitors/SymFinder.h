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

    SymFinder(Type type, Sym *curr, std::vector<Sym*> &result);

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(SubscriptNode &node) override;

    static void find(Type type, Sym *curr, Node *node, std::vector<Sym*> &result);

private:
    Type type;

    Sym *curr;
    std::vector<Sym*> &r;
};

#endif // SYMFINDER_H

#ifndef ADDRGENERATOR_H
#define ADDRGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;
class Node;

class AddrGenerator : public Visitor
{
public:
    AddrGenerator(Context &c, std::ostream &os);

    bool result() const { return ok; }

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(ProxyCallNode &node) override;
    virtual void visit(ConstructNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(ThisNode &node) override;

    static void generate(Context &c, std::ostream &os, Node *node);

private:
    Context &c;
    std::ostream &os;

    bool ok;
};

#endif // ADDRGENERATOR_H

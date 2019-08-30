#ifndef TAKESADDRVISITOR_H
#define TAKESADDRVISITOR_H

#include "visitors/Visitor.h"

class Node;

class TakesAddrVisitor : public Visitor
{
public:
    TakesAddrVisitor();

    bool result() const { return r; }

    virtual void visit(AddrOfNode &node) override;

    static bool examine(Node &node);

private:
    bool r;
};

#endif // TAKESADDRVISITOR_H

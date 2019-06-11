#ifndef SCOPENODE_H
#define SCOPENODE_H

#include "nodes/Node.h"

class Sym;

class ScopeNode : public Node
{
public:
    ScopeNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
    NodePtr block;
};

#endif // SCOPENODE_H

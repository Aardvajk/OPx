#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class Sym;

class VarNode : public Node
{
public:
    VarNode(Location location, Sym *sym, NodePtr value) : Node(location), sym(sym), value(value) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
    NodePtr value;
};

#endif // VARNODE_H

#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class Sym;

class VarNode : public Node
{
public:
    VarNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
};

#endif // VARNODE_H

#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class Sym;

class FuncNode : public Node
{
public:
    FuncNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
    NodePtr block;
};

#endif // FUNCNODE_H

#ifndef FUNCDECNODE_H
#define FUNCDECNODE_H

#include "nodes/Node.h"

class Sym;

class FuncDecNode : public Node
{
public:
    FuncDecNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
};

#endif // FUNCDECNODE_H

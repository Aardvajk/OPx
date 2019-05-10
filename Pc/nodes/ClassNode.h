#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "nodes/Node.h"

class Sym;

class ClassNode : public Node
{
public:
    ClassNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
    NodePtr block;
};

#endif // CLASSNODE_H

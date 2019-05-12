#ifndef NAMESPACENODE_H
#define NAMESPACENODE_H

#include "nodes/Node.h"

class Sym;

class NamespaceNode : public Node
{
public:
    NamespaceNode(Location location, Sym *sym) : Node(location), sym(sym) { }

    virtual void accept(Visitor &v) override;

    Sym *sym;
    NodePtr block;
};

#endif // NAMESPACENODE_H

#ifndef PROXYCALLNODE_H
#define PROXYCALLNODE_H

#include "nodes/Node.h"

class Sym;

class ProxyCallNode : public Node
{
public:
    ProxyCallNode(Location location, Sym *sym, NodePtr thisNode = { }) : Node(location), sym(sym), thisNode(thisNode) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Sym *sym;
    NodePtr thisNode;
    NodeList params;
};

#endif // PROXYCALLNODE_H

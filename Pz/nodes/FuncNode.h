#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr name;
    NodeList args;
    NodePtr type;
    NodePtr body;
};

#endif // FUNCNODE_H

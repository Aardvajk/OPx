#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name), constMethod(false) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr name;
    NodeList args;
    NodePtr type;
    NodePtr body;
    bool constMethod;
};

#endif // FUNCNODE_H

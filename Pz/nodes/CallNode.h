#ifndef CALLNODE_H
#define CALLNODE_H

#include "nodes/Node.h"

class CallNode : public Node
{
public:
    CallNode(Location location, NodePtr target) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr target;
    NodeList params;
};

#endif // CALLNODE_H

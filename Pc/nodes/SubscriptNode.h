#ifndef SUBSCRIPTNODE_H
#define SUBSCRIPTNODE_H

#include "nodes/Node.h"

class SubscriptNode : public Node
{
public:
    SubscriptNode(Location location, NodePtr target) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;

    NodePtr target;
    NodePtr expr;
};

#endif // SUBSCRIPTNODE_H

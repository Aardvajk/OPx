#ifndef SUBSCRIPTNODE_H
#define SUBSCRIPTNODE_H

#include "nodes/Node.h"

class SubscriptNode : public Node
{
public:
    SubscriptNode(Location location, NodePtr target) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr target;
    NodeList params;
};

#endif // SUBSCRIPTNODE_H

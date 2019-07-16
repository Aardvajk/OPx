#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "nodes/Node.h"

class AssignNode : public Node
{
public:
    AssignNode(Location location, NodePtr target) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;

    NodePtr target;
    NodePtr expr;
};

#endif // ASSIGNNODE_H

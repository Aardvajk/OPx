#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "nodes/Node.h"

class AssignNode : public Node
{
public:
    AssignNode(Location location, NodePtr target, NodePtr expr = { }) : Node(location), target(target) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr target;
    NodePtr expr;
};

#endif // ASSIGNNODE_H

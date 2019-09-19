#ifndef DEREFNODE_H
#define DEREFNODE_H

#include "nodes/Node.h"

class DerefNode : public Node
{
public:
    explicit DerefNode(Location location, NodePtr expr = { }) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr expr;
};

#endif // DEREFNODE_H

#ifndef ADDROFNODE_H
#define ADDROFNODE_H

#include "nodes/Node.h"

class AddrOfNode : public Node
{
public:
    explicit AddrOfNode(Location location, NodePtr expr = { }) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr expr;
};

#endif // ADDROFNODE_H

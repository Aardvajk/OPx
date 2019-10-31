#ifndef RETURNNODE_H
#define RETURNNODE_H

#include "nodes/Node.h"

class ReturnNode : public Node
{
public:
    explicit ReturnNode(Location location, NodePtr expr = { }) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr expr;
};

#endif // RETURNNODE_H

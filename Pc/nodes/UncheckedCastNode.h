#ifndef UNCHECKEDCASTNODE_H
#define UNCHECKEDCASTNODE_H

#include "nodes/Node.h"

class UncheckedCastNode : public Node
{
public:
    explicit UncheckedCastNode(Location location, NodePtr type = { }, NodePtr expr = { }) : Node(location), type(type), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr type;
    NodePtr expr;
};

#endif // UNCHECKEDCASTNODE_H

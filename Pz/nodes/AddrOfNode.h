#ifndef ADDROFNODE_H
#define ADDROFNODE_H

#include "nodes/Node.h"

class AddrOfNode : public Node
{
public:
    AddrOfNode(Location location, NodePtr expr = { }) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr expr;
};

#endif // ADDROFNODE_H

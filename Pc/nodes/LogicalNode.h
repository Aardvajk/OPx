#ifndef LOGICALNODE_H
#define LOGICALNODE_H

#include "operators/Operators.h"

#include "nodes/Node.h"

class LogicalNode : public Node
{
public:
    LogicalNode(Location location, Operators::Type op, NodePtr left, NodePtr right) : Node(location), op(op), left(left), right(right) { }

    virtual void accept(Visitor &v) override;

    Operators::Type op;

    NodePtr left;
    NodePtr right;
};

#endif // LOGICALNODE_H

#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "nodes/Node.h"

#include "operators/Operators.h"

class BinaryNode : public Node
{
public:
    BinaryNode(Location location, Operators::Type op, NodePtr left, NodePtr right) : Node(location), op(op), left(left), right(right) { }

    virtual void accept(Visitor &v) override;

    Operators::Type op;

    NodePtr left;
    NodePtr right;
};

#endif // BINARYNODE_H

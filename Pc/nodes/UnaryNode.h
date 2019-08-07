#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "operators/Operators.h"

#include "nodes/Node.h"

class UnaryNode : public Node
{
public:
    UnaryNode(Location location, Operators::Type op) : Node(location), op(op) { }

    virtual void accept(Visitor &v) override;

    Operators::Type op;
    NodePtr expr;
};

#endif // UNARYNODE_H

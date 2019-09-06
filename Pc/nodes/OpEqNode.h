#ifndef OPEQNODE_H
#define OPEQNODE_H

#include "operators/Operators.h"

#include "nodes/Node.h"

class OpEqNode : public Node
{
public:
    OpEqNode(Location location, Operators::Type op, NodePtr target) : Node(location), op(op), target(target) { }

    virtual void accept(Visitor &v) override;

    Operators::Type op;
    NodePtr target;
    NodePtr expr;
};

#endif // OPEQNODE_H

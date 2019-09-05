#ifndef INCDECNODE_H
#define INCDECNODE_H

#include "operators/Operators.h"

#include "nodes/Node.h"

class IncDecNode : public Node
{
public:
    IncDecNode(Location location, Operators::Type op, NodePtr target = { }) : Node(location), op(op), target(target) { }

    virtual void accept(Visitor &v) override;

    Operators::Type op;
    NodePtr target;
};

#endif // INCDECNODE_H

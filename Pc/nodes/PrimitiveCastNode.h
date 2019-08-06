#ifndef PRIMITIVECASTNODE_H
#define PRIMITIVECASTNODE_H

#include "nodes/Node.h"

class Type;

class PrimitiveCastNode : public Node
{
public:
    PrimitiveCastNode(Location location, Type *type, NodePtr expr = { }) : Node(location), type(type), expr(expr) { }

    virtual void accept(Visitor &v) override;

    Type *type;
    NodePtr expr;
};

#endif // PRIMITIVECASTNODE_H

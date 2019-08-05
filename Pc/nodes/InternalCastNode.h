#ifndef INTERNALCASTNODE_H
#define INTERNALCASTNODE_H

#include "nodes/Node.h"

class Type;

class InternalCastNode : public Node
{
public:
    InternalCastNode(Location location, Type *type, NodePtr expr) : Node(location), type(type), expr(expr) { }

    virtual void accept(Visitor &v) override;

    Type *type;
    NodePtr expr;
};

#endif // INTERNALCASTNODE_H

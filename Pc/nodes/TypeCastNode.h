#ifndef TYPECASTNODE_H
#define TYPECASTNODE_H

#include "nodes/Node.h"

class Type;

class TypeCastNode : public Node
{
public:
    TypeCastNode(Location location, Type *type, NodePtr expr) : Node(location), type(type), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Type *type;
    NodePtr expr;
};

#endif // TYPECASTNODE_H

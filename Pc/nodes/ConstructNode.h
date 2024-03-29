#ifndef CONSTRUCTNODE_H
#define CONSTRUCTNODE_H

#include "nodes/Node.h"

class Type;

class ConstructNode : public Node
{
public:
    ConstructNode(Location location, Type *type) : Node(location), type(type) { }
    ConstructNode(Location location, Type *type, NodeList &params) : Node(location), type(type), params(params) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Type *type;
    NodeList params;
    NodePtr target;
};

#endif // CONSTRUCTNODE_H

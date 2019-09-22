#ifndef CONSTRUCTNODE_H
#define CONSTRUCTNODE_H

#include "nodes/Node.h"

class Type;

class ConstructNode : public Node
{
public:
    ConstructNode(Location location, Type *type, NodeList &params) : Node(location), type(type), params(params) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Type *type;
    NodeList params;
};

#endif // CONSTRUCTNODE_H

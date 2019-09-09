#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "common/Object.h"

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name), constMethod(false) { }

    virtual void accept(Visitor &v) override;

    NodePtr name;
    NodePtr type;
    NodeList args;
    bool constMethod;
    NodeList inits;
    NodePtr initialisers;
    NodePtr body;
    Object::Entity::Flags flags;
};

#endif // FUNCNODE_H

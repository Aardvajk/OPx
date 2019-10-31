#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

#include "generics/GenericParams.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name), constMethod(false) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr name;
    GenericParams generics;
    NodeList args;
    NodeList inits;
    NodePtr type;
    NodePtr body;

    bool constMethod;
};

#endif // FUNCNODE_H

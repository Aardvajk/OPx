#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name), constMethod(false), autoGen(false), globalInit(false) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr name;
    NodeList args;
    NodeList inits;
    NodePtr type;
    NodePtr body;

    bool constMethod;
    bool autoGen;
    bool globalInit;
};

#endif // FUNCNODE_H

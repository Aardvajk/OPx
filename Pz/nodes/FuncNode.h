#ifndef FUNCNODE_H
#define FUNCNODE_H

#include "nodes/Node.h"

class FuncNode : public Node
{
public:
    FuncNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodeList genericTags;
    NodeList args;
    NodePtr type;
    NodePtr body;
};

#endif // FUNCNODE_H

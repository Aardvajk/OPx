#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "nodes/Node.h"

class ClassNode : public Node
{
public:
    ClassNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodeList genericTags;
    NodePtr body;
};

#endif // CLASSNODE_H

#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "nodes/Node.h"

class ClassNode : public Node
{
public:
    ClassNode(Location location, NodePtr name, NodePtr body = { }) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr name;
    NodePtr body;
};

#endif // CLASSNODE_H

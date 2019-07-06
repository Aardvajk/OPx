#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "nodes/Node.h"

class ClassNode : public Node
{
public:
    ClassNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;

    NodePtr name;
    NodePtr block;
};

#endif // CLASSNODE_H

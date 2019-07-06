#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;

    NodePtr name;
};

#endif // TYPENODE_H

#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class VarNode : public Node
{
public:
    VarNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;

    NodePtr name;
    NodePtr type;
    NodePtr value;
};

#endif // VARNODE_H

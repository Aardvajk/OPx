#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class VarNode : public Node
{
public:
    VarNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr name;
    NodePtr type;
};

#endif // VARNODE_H

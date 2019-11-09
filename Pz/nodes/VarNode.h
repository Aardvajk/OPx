#ifndef VARNODE_H
#define VARNODE_H

#include "nodes/Node.h"

class VarNode : public Node
{
public:
    VarNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodePtr type;
};

#endif // VARNODE_H

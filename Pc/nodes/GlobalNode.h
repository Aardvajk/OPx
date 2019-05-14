#ifndef GLOBALNODE_H
#define GLOBALNODE_H

#include "nodes/Node.h"

class GlobalNode : public Node
{
public:
    GlobalNode(Location location, NodePtr child) : Node(location), child(std::move(child)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string text() const override;

    NodePtr child;
};

#endif // GLOBALNODE_H

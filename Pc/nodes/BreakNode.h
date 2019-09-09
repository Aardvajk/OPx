#ifndef BREAKNODE_H
#define BREAKNODE_H

#include "nodes/Node.h"

class BreakNode : public Node
{
public:
    explicit BreakNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
};

#endif // BREAKNODE_H

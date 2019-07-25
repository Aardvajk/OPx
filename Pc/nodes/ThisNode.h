#ifndef THISNODE_H
#define THISNODE_H

#include "nodes/Node.h"

class ThisNode : public Node
{
public:
    explicit ThisNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
};

#endif // THISNODE_H

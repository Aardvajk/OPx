#ifndef THISNODE_H
#define THISNODE_H

#include "nodes/Node.h"

class ThisNode : public Node
{
public:
    explicit ThisNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;
};

#endif // THISNODE_H

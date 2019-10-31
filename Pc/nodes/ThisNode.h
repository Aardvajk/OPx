#ifndef THISNODE_H
#define THISNODE_H

#include "nodes/Node.h"

class ThisNode : public Node
{
public:
    explicit ThisNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;
};

#endif // THISNODE_H

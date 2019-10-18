#ifndef UNCHECKEDCASTNODE_H
#define UNCHECKEDCASTNODE_H

#include "nodes/Node.h"

class UncheckedCastNode : public Node
{
public:
    explicit UncheckedCastNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr type;
    NodePtr expr;
};

#endif // UNCHECKEDCASTNODE_H

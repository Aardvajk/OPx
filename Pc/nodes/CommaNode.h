#ifndef COMMANODE_H
#define COMMANODE_H

#include "nodes/Node.h"

class CommaNode : public Node
{
public:
    CommaNode(Location location, NodePtr first, NodePtr second) : Node(location), first(first), second(second) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr first;
    NodePtr second;
};

#endif // COMMANODE_H

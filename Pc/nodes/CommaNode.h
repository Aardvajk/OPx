#ifndef COMMANODE_H
#define COMMANODE_H

#include "nodes/Node.h"

class CommaNode : public Node
{
public:
    CommaNode(Location location, NodePtr left, NodePtr right) : Node(location), left(left), right(right) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr left;
    NodePtr right;
};

#endif // COMMANODE_H

#ifndef IFNODE_H
#define IFNODE_H

#include "nodes/Node.h"

class IfNode : public Node
{
public:
    explicit IfNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr expr;
    NodePtr body;
    NodePtr elseBody;
};

#endif // IFNODE_H

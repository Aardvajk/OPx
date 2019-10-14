#ifndef TERNARYNODE_H
#define TERNARYNODE_H

#include "nodes/Node.h"

class TernaryNode : public Node
{
public:
    TernaryNode(Location location, NodePtr expr) : Node(location), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr expr;
    NodePtr left;
    NodePtr right;
};

#endif // TERNARYNODE_H

#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "nodes/Node.h"

class ExprNode : public Node
{
public:
    ExprNode(Location location, NodePtr node) : Node(location), node(node) { }

    virtual void accept(Visitor &v) override;

    NodePtr node;
};

#endif // EXPRNODE_H

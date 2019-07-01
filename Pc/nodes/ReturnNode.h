#ifndef RETURNNODE_H
#define RETURNNODE_H

#include "nodes/Node.h"

class ReturnNode : public Node
{
public:
    ReturnNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;

    NodePtr expr;
};

#endif // RETURNNODE_H

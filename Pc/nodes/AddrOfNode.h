#ifndef ADDROFNODE_H
#define ADDROFNODE_H

#include "nodes/Node.h"

class AddrOfNode : public Node
{
public:
    explicit AddrOfNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;

    NodePtr expr;
};

#endif // ADDROFNODE_H

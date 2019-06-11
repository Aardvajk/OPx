#ifndef ADDROFNODE_H
#define ADDROFNODE_H

#include "nodes/Node.h"

class AddrOfNode : public Node
{
public:
    explicit AddrOfNode(Location location, NodePtr node);

    virtual void accept(Visitor &visitor);

    NodePtr node;
};

#endif // ADDROFNODE_H

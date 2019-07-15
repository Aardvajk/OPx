#ifndef IDNODE_H
#define IDNODE_H

#include "nodes/Node.h"

class IdNode : public Node
{
public:
    IdNode(Location location, std::string name, NodePtr child = { }) : Node(location), name(std::move(name)), child(child) { }

    virtual void accept(Visitor &v) override;

    std::string name;
    NodePtr child;
};

#endif // IDNODE_H

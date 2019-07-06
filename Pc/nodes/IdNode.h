#ifndef IDNODE_H
#define IDNODE_H

#include "nodes/Node.h"

class IdNode : public Node
{
public:
    IdNode(Location location, std::string name) : Node(location), name(std::move(name)) { }

    virtual void accept(Visitor &v) override;

    std::string name;
};

#endif // IDNODE_H

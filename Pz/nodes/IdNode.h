#ifndef IDNODE_H
#define IDNODE_H

#include "nodes/Node.h"

class IdNode : public Node
{
public:
    IdNode(Location location, NodePtr parent, std::string name) : Node(location), parent(parent), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    NodePtr parent;
    std::string name;
};

#endif // IDNODE_H

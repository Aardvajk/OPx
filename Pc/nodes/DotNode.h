#ifndef DOTNODE_H
#define DOTNODE_H

#include "nodes/Node.h"

#include <string>

class DotNode : public Node
{
public:
    DotNode(Location location, std::string name, NodePtr child) : Node(location), name(std::move(name)), child(std::move(child)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string text() const override;

    std::string name;
    NodePtr child;
};

#endif // DOTNODE_H

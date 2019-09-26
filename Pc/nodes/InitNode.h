#ifndef INITNODE_H
#define INITNODE_H

#include "nodes/Node.h"

class InitNode : public Node
{
public:
    InitNode(Location location, std::string name) : Node(location), name(std::move(name)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    std::string name;
    NodeList params;
};

#endif // INITNODE_H

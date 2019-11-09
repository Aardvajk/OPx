#ifndef NAMESPACENODE_H
#define NAMESPACENODE_H

#include "nodes/Node.h"

class NamespaceNode : public Node
{
public:
    NamespaceNode(Location location, NodePtr name) : Node(location), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    NodePtr name;
    NodePtr body;
};

#endif // NAMESPACENODE_H

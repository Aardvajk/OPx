#ifndef NAMESPACENODE_H
#define NAMESPACENODE_H

#include "nodes/Node.h"

class NamespaceNode : public Node
{
public:
    explicit NamespaceNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr name;
    NodePtr body;
};

#endif // NAMESPACENODE_H

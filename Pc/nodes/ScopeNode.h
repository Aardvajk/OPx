#ifndef SCOPENODE_H
#define SCOPENODE_H

#include "nodes/Node.h"

class ScopeNode : public Node
{
public:
    explicit ScopeNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr body;
};

#endif // SCOPENODE_H

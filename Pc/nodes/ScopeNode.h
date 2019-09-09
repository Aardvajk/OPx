#ifndef SCOPENODE_H
#define SCOPENODE_H

#include "nodes/Node.h"

class ScopeNode : public Node
{
public:
    explicit ScopeNode(Location location, bool loop = false) : Node(location), loop(loop) { }

    virtual void accept(Visitor &v) override;

    bool loop;
    NodePtr body;
};

#endif // SCOPENODE_H

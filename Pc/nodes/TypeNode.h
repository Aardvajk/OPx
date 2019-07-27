#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location) : Node(location), ptr(0), function(false) { }

    virtual void accept(Visitor &v) override;

    unsigned ptr;
    bool function;

    NodePtr name;

    NodeList args;
    NodePtr returnType;

    NodePtr sub;
};

#endif // TYPENODE_H

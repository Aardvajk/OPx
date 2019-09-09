#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location) : Node(location), constant(false), ref(false), constMethod(false), ptr(0), function(false) { }

    virtual void accept(Visitor &v) override;

    bool constant;
    bool ref;
    bool constMethod;

    unsigned ptr;
    bool function;

    NodePtr name;

    NodeList args;
    NodePtr returnType;

    NodePtr sub;
};

#endif // TYPENODE_H

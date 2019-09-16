#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location) : Node(location), constant(false), ref(false), ptr(0) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    bool constant;
    bool ref;

    std::size_t ptr;

    NodePtr name;

    NodeList args;
    NodePtr returnType;
};

#endif // TYPENODE_H

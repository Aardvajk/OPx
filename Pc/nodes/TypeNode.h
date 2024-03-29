#ifndef TYPENODE_H
#define TYPENODE_H

#include "nodes/Node.h"

class TypeNode : public Node
{
public:
    explicit TypeNode(Location location, NodePtr name = { }) : Node(location), function(false), constant(false), ref(false), ptr(0), name(name) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    bool function;
    bool constant;
    bool ref;

    std::size_t ptr;

    NodePtr name;

    NodeList args;
    NodePtr returnType;
};

#endif // TYPENODE_H

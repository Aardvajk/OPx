#ifndef LITERALNODES_H
#define LITERALNODES_H

#include "nodes/Node.h"

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;
    virtual NodePtr cloneDetail() const override;

    int value;
};

#endif // LITERALNODES_H

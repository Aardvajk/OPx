#ifndef LITERALNODES_H
#define LITERALNODES_H

#include "nodes/Node.h"

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    int value;
};

class BoolLiteralNode : public Node
{
public:
    BoolLiteralNode(Location location, bool value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    bool value;
};

class StringLiteralNode : public Node
{
public:
    StringLiteralNode(Location location, std::string value) : Node(location), value(std::move(value)) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    std::string value;
};

#endif // LITERALNODES_H

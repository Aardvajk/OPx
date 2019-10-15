#ifndef LITERALNODES_H
#define LITERALNODES_H

#include "nodes/Node.h"

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(Location location, char value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    char value;
};

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    int value;
};

class SizeLiteralNode : public Node
{
public:
    SizeLiteralNode(Location location, std::size_t value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    std::size_t value;
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

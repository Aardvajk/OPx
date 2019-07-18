#ifndef LITERALNODES_H
#define LITERALNODES_H

#include "nodes/Node.h"

class NullLiteralNode : public Node
{
public:
    explicit NullLiteralNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
};

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(Location location, char value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;

    char value;
};

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;

    int value;
};

class BoolLiteralNode : public Node
{
public:
    BoolLiteralNode(Location location, bool value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;

    bool value;
};

#endif // LITERALNODES_H

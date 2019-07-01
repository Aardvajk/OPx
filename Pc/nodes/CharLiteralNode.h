#ifndef CHARLITERALNODE_H
#define CHARLITERALNODE_H

#include "nodes/Node.h"

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(Location location, char value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;

    char value;
};

#endif // CHARLITERALNODE_H

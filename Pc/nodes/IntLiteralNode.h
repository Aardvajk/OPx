#ifndef INTLITERALNODE_H
#define INTLITERALNODE_H

#include "nodes/Node.h"

class IntLiteralNode : public Node
{
public:
    IntLiteralNode(Location location, int value) : Node(location), value(value) { }

    virtual void accept(Visitor &v) override;

    int value;
};

#endif // INTLITERALNODE_H

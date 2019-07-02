#ifndef NULLLITERALNODE_H
#define NULLLITERALNODE_H

#include "nodes/Node.h"

class NullLiteralNode : public Node
{
public:
    explicit NullLiteralNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
};

#endif // NULLLITERALNODE_H

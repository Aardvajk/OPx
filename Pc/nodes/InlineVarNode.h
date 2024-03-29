#ifndef INLINEVARNODE_H
#define INLINEVARNODE_H

#include "nodes/Node.h"

class InlineVarNode : public Node
{
public:
    explicit InlineVarNode(Location location, NodePtr body = { }) : Node(location), body(body) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr body;
};

#endif // INLINEVARNODE_H

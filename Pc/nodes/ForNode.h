#ifndef FORNODE_H
#define FORNODE_H

#include "nodes/Node.h"

class ForNode : public Node
{
public:
    explicit ForNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    NodePtr init;
    NodePtr cond;
    NodePtr post;
    NodePtr body;
};

#endif // FORNODE_H

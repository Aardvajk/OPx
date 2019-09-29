#ifndef WHILENODE_H
#define WHILENODE_H

#include "nodes/Node.h"

class WhileNode : public Node
{
public:
    explicit WhileNode(Location location) : Node(location) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    NodePtr expr;
    NodePtr body;
};

#endif // WHILENODE_H

#ifndef LOGICALNODE_H
#define LOGICALNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class LogicalNode : public Node
{
public:
    LogicalNode(Location location, const Token &token, NodePtr left, NodePtr right) : Node(location), token(token), left(left), right(right) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Token token;

    NodePtr left;
    NodePtr right;
};

#endif // LOGICALNODE_H

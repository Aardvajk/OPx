#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class UnaryNode : public Node
{
public:
    UnaryNode(Location location, const Token &token, NodePtr expr = { }) : Node(location), token(token), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Token token;
    NodePtr expr;
};

#endif // UNARYNODE_H

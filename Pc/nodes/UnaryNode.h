#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class UnaryNode : public Node
{
public:
    UnaryNode(Location location, const Token &token) : Node(location), token(token) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Token token;
    NodePtr expr;
};

#endif // UNARYNODE_H

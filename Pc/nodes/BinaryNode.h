#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class BinaryNode : public Node
{
public:
    BinaryNode(Location location, const Token &token, NodePtr left, NodePtr right) : Node(location), token(token), left(left), right(right) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Token token;
    NodePtr left;
    NodePtr right;
};

#endif // BINARYNODE_H

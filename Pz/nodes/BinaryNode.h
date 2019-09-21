#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class BinaryNode : public Node
{
public:
    BinaryNode(Location location, const Token &type, NodePtr left, NodePtr right) : Node(location), type(type), left(left), right(right) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    Token type;
    NodePtr left;
    NodePtr right;
};

#endif // BINARYNODE_H

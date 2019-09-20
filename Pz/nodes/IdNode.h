#ifndef IDNODE_H
#define IDNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class IdNode : public Node
{
public:
    IdNode(Location location, NodePtr parent, std::string name) : Node(location), parent(parent), name(std::move(name)), special(Token::Type::Invalid) { }

    virtual void accept(Visitor &v) override;
    virtual const char *classname() const override;

    NodePtr parent;
    std::string name;

    Token::Type special;
};

#endif // IDNODE_H

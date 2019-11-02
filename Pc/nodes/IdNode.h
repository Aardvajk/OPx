#ifndef IDNODE_H
#define IDNODE_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class Type;

class IdNode : public Node
{
public:
    IdNode(Location location, NodePtr parent, std::string name) : Node(location), parent(parent), name(std::move(name)), special(Token::Type::Invalid), arrow(false) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    static NodePtr create(Location location, const std::vector<std::string> &names);

    NodePtr parent;
    std::string name;

    NodeList generics;

    Token::Type special;
    NodePtr op;
    bool arrow;
};

#endif // IDNODE_H

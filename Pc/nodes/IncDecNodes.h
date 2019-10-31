#ifndef INCDECNODES_H
#define INCDECNODES_H

#include "scanner/Token.h"

#include "nodes/Node.h"

class PreIncDecNode : public Node
{
public:
    PreIncDecNode(Location location, const Token &token, NodePtr expr = { }) : Node(location), token(token) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Token token;
    NodePtr expr;
};

class PostIncDecNode : public Node
{
public:
    PostIncDecNode(Location location, const Token &token, NodePtr expr) : Node(location), token(token), expr(expr) { }

    virtual void accept(Visitor &v) override;
    virtual std::string classname() const override;

    virtual Node *clone() const override;

    Token token;
    NodePtr expr;
};

#endif // INCDECNODES_H

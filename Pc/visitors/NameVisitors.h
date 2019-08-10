#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "scanner/Token.h"

#include "visitors/Visitor.h"

#include <string>

class Node;

namespace NameVisitors
{

class PrettyName : public Visitor
{
public:
    PrettyName();

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(NullLiteralNode &node) override;
    virtual void visit(IntLiteralNode &node) override;
    virtual void visit(CharLiteralNode &node) override;
    virtual void visit(BoolLiteralNode &node) override;
    virtual void visit(SizeLiteralNode &node) override;
    virtual void visit(StringLiteralNode &node) override;
    virtual void visit(ThisNode &node) override;

private:
    std::string r;
};

std::string prettyName(Node *node);

class IsNameSimple : public Visitor
{
public:
    IsNameSimple();

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    bool r;
};

bool isNameSimple(Node *node);

class LastIdOfName : public Visitor
{
public:
    LastIdOfName();

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    std::string r;
};

std::string lastIdOfName(Node *node);

class IsNameSpecial : public Visitor
{
public:
    IsNameSpecial();

    Token::Type result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    Token::Type r;
};

Token::Type isNameSpecial(Node *node);

}

#endif // NAMEVISITORS_H

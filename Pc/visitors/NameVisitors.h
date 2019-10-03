#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "scanner/Location.h"
#include "scanner/Token.h"

#include "visitors/Visitor.h"

#include <string>

class Context;
class Node;
class Type;

namespace NameVisitors
{

class IsNameSimple : public Visitor
{
public:
    IsNameSimple();

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    bool r;
};

class LastIdOfName : public Visitor
{
public:
    LastIdOfName();

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    std::string r;
};

class SpecialName : public Visitor
{
public:
    SpecialName();

    Token::Type result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    Token::Type r;
};

class ResolveOpName : public Visitor
{
public:
    ResolveOpName(Context &c);

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    Context &c;
    Type *r;
};

class ResolveOpType : public Visitor
{
public:
    ResolveOpType(Context &c, IdNode &id);

    Type *result() const { return r; }

    virtual void visit(TypeNode &node) override;
    virtual void visit(TextNode &node) override;

private:
    Context &c;
    IdNode &id;
    Type *r;
};

std::string assertSimpleName(Context &c, Node *node);
std::string assertUniqueName(Context &c, Node *node);

std::string assertSimpleUniqueName(Context &c, Node *node);

}

#endif // NAMEVISITORS_H

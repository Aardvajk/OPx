#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "scanner/Location.h"
#include "scanner/Token.h"

#include "visitors/Visitor.h"

#include <string>

class Context;
class Node;

namespace NameVisitors
{

class IsNameSimple : public Visitor
{
public:
    IsNameSimple() : r(false) { }

    bool result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    bool r;
};

class LastIdOfName : public Visitor
{
public:
    LastIdOfName(){ }

    std::string result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    std::string r;
};

std::string assertSimpleName(Context &c, Node *node);
std::string assertUniqueName(Context &c, Node *node);

std::string assertSimpleUniqueName(Context &c, Node *node);

class GenericTagName : public Visitor
{
public:
    GenericTagName(){ }

    std::string result() const { return r; }

    virtual void visit(GenericTagNode &node) override;

private:
    std::string r;
};

}

#endif // NAMEVISITORS_H

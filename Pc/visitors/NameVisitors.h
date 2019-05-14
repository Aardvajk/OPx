#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "visitors/Visitor.h"

#include <string>

class Node;

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

bool isNameSimple(Node *node);

class LastIdOfName : public Visitor
{
public:
    LastIdOfName();

    std::string result() const { return r; }

    virtual void visit(GlobalNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    std::string r;
};

std::string lastIdOfName(Node *node);

}

#endif // NAMEVISITORS_H

#ifndef NAMEVISITORS_H
#define NAMEVISITORS_H

#include "visitors/Visitor.h"

class Node;

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

#endif // NAMEVISITORS_H

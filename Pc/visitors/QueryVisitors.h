#ifndef QUERYVISITORS_H
#define QUERYVISITORS_H

#include "visitors/Visitor.h"

#include "nodes/Node.h"

class Type;

namespace QueryVisitors
{

class DirectType : public Visitor
{
public:
    DirectType();

    Type *result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    Type *r;
};

class GetParent : public Visitor
{
public:
    GetParent();

    NodePtr result() const { return r; }

    virtual void visit(IdNode &node) override;

private:
    NodePtr r;
};

class GetConstructNode : public Visitor
{
public:
    GetConstructNode();

    ConstructNode *result() const { return r; }

    virtual void visit(ConstructNode &node) override;

private:
    ConstructNode *r;
};

}

#endif // QUERYVISITORS_H

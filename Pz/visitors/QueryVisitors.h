#ifndef QUERYVISITORS_H
#define QUERYVISITORS_H

#include "visitors/Visitor.h"

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

}

#endif // QUERYVISITORS_H

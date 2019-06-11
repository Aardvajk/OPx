#ifndef STOREVISITOR_H
#define STOREVISITOR_H

#include "visitors/Visitor.h"

class Context;

class StoreVisitor : public Visitor
{
public:
    StoreVisitor(Context &c);

    virtual void visit(IdNode &node) override;

private:
    Context &c;
};

#endif // STOREVISITOR_H

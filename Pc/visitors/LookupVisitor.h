#ifndef LOOKUPVISITOR_H
#define LOOKUPVISITOR_H

#include "visitors/Visitor.h"

class Context;

class LookupVisitor : public Visitor
{
public:
    explicit LookupVisitor(Context &c);

    virtual void visit(GlobalNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    Context &c;
};

#endif // LOOKUPVISITOR_H

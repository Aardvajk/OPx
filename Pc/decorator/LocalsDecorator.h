#ifndef LOCALSDECORATOR_H
#define LOCALSDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class LocalsDecorator : public Visitor
{
public:
    explicit LocalsDecorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;

private:
    Context &c;
};

#endif // LOCALSDECORATOR_H

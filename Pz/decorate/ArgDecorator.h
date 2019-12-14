#ifndef ARGDECORATOR_H
#define ARGDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class ArgDecorator : public Visitor
{
public:
    explicit ArgDecorator(Context &c);

    virtual void visit(VarNode &node) override;

private:
    Context &c;
};

#endif // ARGDECORATOR_H

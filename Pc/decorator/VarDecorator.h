#ifndef VARDECORATOR_H
#define VARDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class VarDecorator : public Visitor
{
public:
    explicit VarDecorator(Context &c);

    virtual void visit(VarNode &node) override;

private:
    Context &c;
};

#endif // VARDECORATOR_H

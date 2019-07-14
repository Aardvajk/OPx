#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class ExprDecorator : public Visitor
{
public:
    explicit ExprDecorator(Context &c);

    virtual void visit(CallNode &node) override;

private:
    Context &c;
};

#endif // EXPRDECORATOR_H

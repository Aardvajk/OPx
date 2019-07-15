#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

#include <pcx/optional.h>

class Context;
class Type;

class ExprDecorator : public Visitor
{
public:
    ExprDecorator(Context &c, const Type *expectedType);

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;

private:
    Context &c;
    const Type *expectedType;
};

#endif // EXPRDECORATOR_H

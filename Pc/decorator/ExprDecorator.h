#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

class Context;
class Type;

class ExprDecorator : public Visitor
{
public:
    ExprDecorator(Context &c, const Type *expectedType);

private:
    Context &c;
    const Type *expectedType;
};

#endif // EXPRDECORATOR_H

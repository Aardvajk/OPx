#ifndef EXPRDECORATOR_H
#define EXPRDECORATOR_H

#include "visitors/Visitor.h"

#include <pcx/optional.h>

class Context;
class Type;
class Node;

class ExprDecorator : public Visitor
{
public:
    ExprDecorator(Context &c, const Type *expectedType);

    virtual void visit(IdNode &node) override;
    virtual void visit(CallNode &node) override;
    virtual void visit(AddrOfNode &node) override;
    virtual void visit(AssignNode &node) override;
    virtual void visit(DerefNode &node) override;
    virtual void visit(UnaryNode &node) override;
    virtual void visit(BinaryNode &node) override;
    virtual void visit(SubscriptNode &node) override;

    static void decorate(Context &c, const Type *expectedType, Node &node);

private:
    Context &c;
    const Type *expectedType;
};

#endif // EXPRDECORATOR_H

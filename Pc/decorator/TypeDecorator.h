#ifndef TYPEDECORATOR_H
#define TYPEDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class TypeDecorator : public Visitor
{
public:
    explicit TypeDecorator(Context &c);

    virtual void visit(TypeNode &node) override;
    virtual void visit(VarNode &node) override;

private:
    Context &c;
};

#endif // TYPEDECORATOR_H

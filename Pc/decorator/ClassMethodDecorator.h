#ifndef CLASSMETHODDECORATOR_H
#define CLASSMETHODDECORATOR_H

#include "visitors/Visitor.h"

class Context;
class Sym;

class ClassMethodDecorator : public Visitor
{
public:
    ClassMethodDecorator(Context &c, Sym *sym);

    virtual void visit(BlockNode &node) override;

private:
    Context &c;
    Sym *sym;
};

#endif // CLASSMETHODDECORATOR_H

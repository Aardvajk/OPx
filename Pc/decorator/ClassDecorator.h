#ifndef CLASSDECORATOR_H
#define CLASSDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class ClassDecorator : public Visitor
{
public:
    explicit ClassDecorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(FuncNode &node) override;

private:
    Context &c;
};

#endif // CLASSDECORATOR_H

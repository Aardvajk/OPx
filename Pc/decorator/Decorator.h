#ifndef DECORATOR_H
#define DECORATOR_H

#include "visitors/Visitor.h"

class Context;

class Decorator : public Visitor
{
public:
    explicit Decorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(PragmaNode &node) override;

private:
    Context &c;
};

#endif // DECORATOR_H

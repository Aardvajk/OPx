#ifndef FUNCDECORATOR_H
#define FUNCDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class FuncDecorator : public Visitor
{
public:
    explicit FuncDecorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;
    virtual void visit(WhileNode &node) override;
    virtual void visit(IfNode &node) override;
    virtual void visit(InitNode &node) override;

private:
    Context &c;
};

#endif // FUNCDECORATOR_H

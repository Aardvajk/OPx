#ifndef FUNCLOWER_H
#define FUNCLOWER_H

#include "visitors/Visitor.h"

class Context;

class FuncLower : public Visitor
{
public:
    explicit FuncLower(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;

private:
    Context &c;
};

#endif // FUNCLOWER_H

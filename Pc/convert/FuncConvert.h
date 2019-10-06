#ifndef FUNCCONVERT_H
#define FUNCCONVERT_H

#include "visitors/Visitor.h"

class Context;

class FuncConvert : public Visitor
{
public:
    explicit FuncConvert(Context &c);

private:
    Context &c;

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;
    virtual void visit(InitNode &node) override;
    virtual void visit(WhileNode &node) override;
    virtual void visit(IfNode &node) override;
    virtual void visit(ForNode &node) override;
};

#endif // FUNCCONVERT_H

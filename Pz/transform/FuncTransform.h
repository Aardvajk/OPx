#ifndef FUNCTRANSFORM_H
#define FUNCTRANSFORM_H

#include "visitors/Visitor.h"

class Context;

class FuncTransform : public Visitor
{
public:
    explicit FuncTransform(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;

private:
    Context &c;
};

#endif // FUNCTRANSFORM_H

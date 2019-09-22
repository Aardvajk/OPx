#ifndef FUNCTRANSFORM_H
#define FUNCTRANSFORM_H

#include "visitors/Visitor.h"

#include <cstdlib>

class Context;

class FuncTransform : public Visitor
{
public:
    explicit FuncTransform(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;

private:
    Context &c;
    std::size_t index;
};

#endif // FUNCTRANSFORM_H

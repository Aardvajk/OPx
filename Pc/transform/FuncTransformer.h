#ifndef FUNCTRANSFORMER_H
#define FUNCTRANSFORMER_H

#include "visitors/Visitor.h"

#include <cstdlib>

class Context;

class FuncTransformer : public Visitor
{
public:
    explicit FuncTransformer(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;
    virtual void visit(WhileNode &node) override;
    virtual void visit(IfNode &node) override;
    virtual void visit(InitNode &node) override;
    virtual void visit(ForNode &node) override;

private:
    Context &c;
    std::size_t index;
};

#endif // FUNCTRANSFORMER_H

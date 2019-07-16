#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "visitors/Visitor.h"

class Context;

class Transformer : public Visitor
{
public:
    explicit Transformer(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ExprNode &node) override;

private:
    Context &c;
};

#endif // TRANSFORMER_H

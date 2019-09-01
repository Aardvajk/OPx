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
    virtual void visit(ClassNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(PragmaNode &node) override;

private:
    Context &c;
};

#endif // TRANSFORMER_H

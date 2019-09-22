#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "visitors/Visitor.h"

class Context;

class Transform : public Visitor
{
public:
    explicit Transform(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(PragmaNode &node) override;

private:
    Context &c;
};

#endif // TRANSFORM_H

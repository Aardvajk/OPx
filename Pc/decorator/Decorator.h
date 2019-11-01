#ifndef DECORATOR_H
#define DECORATOR_H

#include "nodes/Node.h"

#include "visitors/Visitor.h"

class Context;

class Decorator : public Visitor
{
public:
    Decorator(Context &c, NodePtr &cn);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(PragmaNode &node) override;

    static void decorate(Context &c, NodePtr node);
    static void decorateFunction(Context &c, FuncNode *node);

private:
    Context &c;
    NodePtr &cn;
};

#endif // DECORATOR_H

#ifndef LOWER_H
#define LOWER_H

#include "visitors/Visitor.h"

class Context;

class Lower : public Visitor
{
public:
    explicit Lower(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(PragmaNode &node) override;

private:
    Context &c;
};

#endif // LOWER_H

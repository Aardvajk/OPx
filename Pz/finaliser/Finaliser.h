#ifndef FINALISER_H
#define FINALISER_H

#include "visitors/Visitor.h"

class Context;

class Finaliser : public Visitor
{
public:
    explicit Finaliser(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(ClassNode &node) override;

private:
    Context &c;
};

#endif // FINALISER_H

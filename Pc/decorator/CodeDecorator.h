#ifndef CODEDECORATOR_H
#define CODEDECORATOR_H

#include "visitors/Visitor.h"

class Context;

class CodeDecorator : public Visitor
{
public:
    explicit CodeDecorator(Context &c);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;

private:
    Context &c;
};

#endif // CODEDECORATOR_H

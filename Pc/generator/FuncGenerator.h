#ifndef FUNCGENERATOR_H
#define FUNCGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class FuncGenerator : public Visitor
{
public:
    FuncGenerator(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;

private:
    Context &c;
    std::ostream &os;
};

#endif // FUNCGENERATOR_H

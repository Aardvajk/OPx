#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class CodeGenerator : public Visitor
{
public:
    CodeGenerator(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ExprNode &node) override;
    virtual void visit(ReturnNode &node) override;

private:
    Context &c;
    std::ostream &os;
};

#endif // CODEGENERATOR_H

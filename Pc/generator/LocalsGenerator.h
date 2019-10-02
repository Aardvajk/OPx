#ifndef LOCALSGENERATOR_H
#define LOCALSGENERATOR_H

#include "visitors/Visitor.h"

#include <iostream>

class Context;

class LocalsGenerator : public Visitor
{
public:
    LocalsGenerator(Context &c, std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(WhileNode &node) override;
    virtual void visit(IfNode &node) override;
    virtual void visit(ForNode &node) override;

private:
    Context &c;
    std::ostream &os;
};

#endif // LOCALSGENERATOR_H

#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "visitors/Visitor.h"

#include <iostream>

class AstPrinter : public Visitor
{
public:
    AstPrinter(std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(TypeNode &node) override;
    virtual void visit(FuncNode &node) override;
    virtual void visit(ScopeNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(VarNode &node) override;
    virtual void visit(IntLiteralNode &node) override;

private:
    std::ostream &tab() const;

    std::ostream &os;
    unsigned tc;
};

#endif // ASTPRINTER_H

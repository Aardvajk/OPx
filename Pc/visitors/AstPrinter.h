#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "visitors/Visitor.h"

#include <iostream>

class AstPrinter : public Visitor
{
public:
    AstPrinter(std::ostream &os);

    virtual void visit(BlockNode &node) override;
    virtual void visit(NamespaceNode &node) override;
    virtual void visit(ClassNode &node) override;
    virtual void visit(GlobalNode &node) override;
    virtual void visit(IdNode &node) override;
    virtual void visit(DotNode &node) override;

private:
    std::ostream &tab() const;

    std::ostream &os;
    unsigned tc;
};

#endif // ASTPRINTER_H
